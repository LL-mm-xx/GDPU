#include <linux/types.h> 
#include <linux/kernel.h> 
#include <linux/delay.h> 
#include <linux/ide.h> 
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/errno.h> 
#include <linux/gpio.h> 
#include <linux/cdev.h> 
#include <linux/device.h> 
#include <linux/of_gpio.h> 
#include <linux/semaphore.h> 
#include <linux/timer.h> 
#include <linux/i2c.h> 
#include <linux/spi/spi.h> 
#include <linux/of.h> 
#include <linux/of_address.h> 
#include <linux/of_gpio.h> 
#include <linux/platform_device.h> 
#include <asm/mach/map.h> 
#include <asm/uaccess.h> 
#include <asm/io.h> 
#include <linux/spi/spi.h> 

#define ipsTft_CNT	1 
#define ipsTft_NAME	"ipsTft" 

#define LCD_W 240 
#define LCD_H 240 

#define WHITE         	 0xFFFF 
#define BLACK         	 0x0000 
#define BLUE         	 0x001F 
#define BRED             0XF81F 
#define GRED 			 0XFFE0 
#define GBLUE			 0X07FF 
#define RED           	 0xF800 
#define MAGENTA       	 0xF81F 
#define GREEN         	 0x07E0 
#define CYAN          	 0x7FFF 
#define YELLOW        	 0xFFE0 
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430  

// u8 buf[9] = { 
// 	RED, GREEN, BLUE, WHITE, BLACK, YELLOW, GRAY, BRRED, CYAN 
// }; 

struct ipsTft_dev { 
	dev_t devid;				/* 设备号 */ 
	struct cdev cdev;			/* cdev */ 
	struct class *class;		/* 类 */ 
	struct device *device;		/* 设备 */ 
	struct device_node	*nd; 	/* 设备节点 */ 
	int major;					/* 主设备号 */ 
	void *private_data;			/* 私有数据 */ 
	int dc_gpio;				/* 片选所使用的GPIO编号 */ 
    int res_gpio;               /* ips屏幕复位引脚 */ 
    int cs_gpio;                /* 共用磁力计的cs */ 
}; 

static struct ipsTft_dev ipsTftdev; 

void ipsTft_reginit(struct ipsTft_dev *dev); 

//1.3寸屏幕 
struct spi_lcd_cmd { 
    u8  reg_addr;  // command 
    u8  len;       //需要从spi_lcd_datas数组里发出数据字节数 
    int delay_ms;  //此命令发送数据完成后，需延时多久 
};

struct spi_lcd_cmd cmds[] = { 
    {0x36, 1, 30}, 
    {0x3A, 1, 30}, 
    {0xB2, 5, 30}, 
    {0xB7, 1, 30}, 
    {0xBB, 1, 30}, 
    {0xC0, 1, 30}, 
    {0xC2, 1, 30}, 
    {0xC3, 1, 30}, 
    {0xC4, 1, 30}, 
    {0xC6, 1, 30}, 
    {0xD0, 2, 30}, 
    {0xE0, 14, 30}, 
    {0xE1, 14, 30}, 
    {0x21, 0, 30}, 
    {0x11, 0, 120}, 
    {0x29, 0, 30}, 
}; 

u8 spi_lcd_datas[] = { 
    0x00, 
    0x05, 
    0x0c,0x0c,0x00,0x33,0x33, 
    0x35, 
    0x19, 
    0x2c, 
    0x01, 
    0x12, 
    0x20, 
    0x0F, 
    0xA4,0xA1, 
    0xD0,0x04,0x0D,0x11,0x13,0x2B,0x3F,0x54,0x4C,0x18,0x0D,0x0B,0x1F,0x23, 
    0xD0,0x04,0x0C,0x11,0x13,0x2C,0x3F,0x44,0x51,0x2F,0x1F,0x1F,0x20,0x23 
}; 
/* 
 * @description	: 向ipsTft多个寄存器写入数据 
 * @param - dev:  ipsTft设备 
 * @param - reg:  要写入的寄存器首地址 
 * @param - val:  要写入的数据缓冲区 
 * @param - len:  要写入的数据长度 
 * @return 	  :   操作结果 
 */ 
static s32 ipsTft_write_regs(struct ipsTft_dev *dev,u8 *buf, u8 len) 
{ 
	int ret; 
	struct spi_message m; 
	struct spi_transfer *t; 
	struct spi_device *spi = (struct spi_device *)dev->private_data; 
	t = kzalloc(sizeof(struct spi_transfer), GFP_KERNEL);	/* 申请内存 */ 
	t->tx_buf = buf;			/* 要写入的数据 */ 
	t->len = len;				/* 写入的字节数 */ 
	spi_message_init(&m);		/* 初始化spi_message */ 
	spi_message_add_tail(t, &m);/* 将spi_transfer添加到spi_message队列 */ 
	ret = spi_sync(spi, &m);	/* 同步发送 */ 
	kfree(t);					/* 释放内存 */ 
	return ret; 
} 
/* 
 * @description	: 向ipsTft指定寄存器写入指定的值，写一个寄存器 
 * @param - dev:  ipsTft设备 
 * @param - reg:  要写的寄存器 
 * @param - data: 要写入的值 
 * @return   :    无 
 */ 
static void ipsTft_write_onereg(struct ipsTft_dev *dev, u8 buf) 
{ 
	ipsTft_write_regs(dev,&buf, 1); 
    //spi_write(dev,&buf, 1); 
} 
/* 
    funciton: 写一个命令 
*/ 
void write_command(struct ipsTft_dev *dev, u8 cmd) 
{ 
    // dc , command:0 
    gpio_set_value(dev->dc_gpio, 0); 
    ipsTft_write_onereg(dev,cmd); 
} 
/* 
    funciton: 写一个数据 
*/ 
void write_data(struct ipsTft_dev *dev, u8 data) 
{ 
    gpio_set_value(dev->dc_gpio, 1); 
    ipsTft_write_onereg(dev,data); 
} 
/* 
    funciton: 写一些数据 
*/ 
static void write_datas(struct ipsTft_dev *dev, int data,int len) 
{ 
    gpio_set_value(dev->dc_gpio, 1); 
    ipsTft_write_regs(dev,(u8 *)&data,len); 
} 
/* 
 * @description		: 打开设备 
 * @param - inode 	: 传递给驱动的inode 
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的私有成员变量 
 * 					  一般在open的时候将private_data向私有设备结构体赋值。 
 * @return 			: 0 成功;其他 失败 
 */ 
static int ipsTft_open(struct inode *inode, struct file *filp) 
{ 
	filp->private_data = &ipsTftdev; /* 设置私有数据 */ 
	// TODO something
	return 0; 
} 
/* 
 * @description		: 关闭/释放设备 
 * @param - filp 	: 要关闭的设备文件(文件描述符) 
 * @return 			: 0 成功;其他 失败 
 */ 
static int ipsTft_release(struct inode *inode, struct file *filp) 
{ 
	return 0; 
} 
/* ipsTft操作函数 */ 
static const struct file_operations ipsTft_ops = { 
	.owner = THIS_MODULE, 
	.open = ipsTft_open, 
	.release = ipsTft_release, 
}; 

void Address_set(struct ipsTft_dev *dev,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2) 
{ 
    write_command(dev,0x2a); 
    write_data(dev,x1>>8); 
    write_data(dev,x1); 
    write_data(dev,x2>>8); 
    write_data(dev,x2); 
    write_command(dev,0x2b); 
    write_data(dev,y1>>8); 
    write_data(dev,y1); 
    write_data(dev,y2>>8); 
    write_data(dev,y2); 
    write_command(dev,0x2C); 
} 
/* 
    刷屏函数 
*/ 
void LCD_Clear(struct ipsTft_dev *dev,u16 Color) 
{ 
	u16 i,j; 
	Address_set(dev,0,0,LCD_W-1,LCD_H-1); 
	write_command(dev,0x2C); 
	for(i=0;i<LCD_W;i++) 
	{ 
		for (j=0;j<LCD_H;j++) 
		{ 
			//write_datas(dev,0xF800,2);	    //全红 
			write_data(dev,Color>>8); 
			write_data(dev,Color); 
		} 
	} 
} 
/* 
 * ipsTft内部寄存器初始化函数 
 * @param  	: 无 
 * @return 	: 无 
 */ 
void ipsTft_reginit(struct ipsTft_dev *dev) 
{ 
    int i, j, n; 
    gpio_set_value(ipsTftdev.res_gpio, 0); 
    mdelay(20); 
    gpio_set_value(ipsTftdev.res_gpio, 1); 
    mdelay(20); 
    n = 0; // n用于记录数据数组spi_lcd_datas的位置 
    //发命令，并发出命令所需的数据 
    for (i = 0; i < ARRAY_SIZE(cmds); i++) //命令 
    { 
        write_command(dev, cmds[i].reg_addr); 
        for (j = 0; j < cmds[i].len; j++) //发出命令后，需要发出的数据 
            if(cmds[i].len!=0) 
                write_data(dev, spi_lcd_datas[n++]); 
        if (cmds[i].delay_ms) //如有延时则延时 
            mdelay(cmds[i].delay_ms); 
    } 
    n=0; 
    LCD_Clear(dev,RED); // 安装驱动模块的时候刷个红色的屏
    printk("ips init finish!\n"); 
} 
 /* 
  * @description     : spi驱动的probe函数，当驱动与 
  *                    设备匹配以后此函数就会执行 
  * @param - client  : spi设备 
  * @param - id      : spi设备ID 
  * 
  */ 
static int ipsTft_probe(struct spi_device *spi) 
{ 
	int ret = 0; 
	
	printk("TFT driver and device was matched!\r\n");
	
	/* 1、构建设备号 */ 
	if (ipsTftdev.major) { 
		ipsTftdev.devid = MKDEV(ipsTftdev.major, 0); 
		register_chrdev_region(ipsTftdev.devid, ipsTft_CNT, ipsTft_NAME); 
	} else { 
		alloc_chrdev_region(&ipsTftdev.devid, 0, ipsTft_CNT, ipsTft_NAME); 
		ipsTftdev.major = MAJOR(ipsTftdev.devid); 
	} 
	/* 2、注册设备 */ 
	cdev_init(&ipsTftdev.cdev, &ipsTft_ops); 
	cdev_add(&ipsTftdev.cdev, ipsTftdev.devid, ipsTft_CNT); 
	/* 3、创建类 */ 
	ipsTftdev.class = class_create(THIS_MODULE, ipsTft_NAME); 
	if (IS_ERR(ipsTftdev.class)) { 
		return PTR_ERR(ipsTftdev.class); 
	} 
	/* 4、创建设备 */ 
	ipsTftdev.device = device_create(ipsTftdev.class, NULL, ipsTftdev.devid, NULL, ipsTft_NAME); 
	if (IS_ERR(ipsTftdev.device)) { 
		return PTR_ERR(ipsTftdev.device); 
	} 
	/* 获取设备树中的 spi0 节点 */
	ipsTftdev.nd = of_find_node_by_path("/soc/spi@01c68000"); 
	if(ipsTftdev.nd == NULL) { 
		printk("spi0 node not find!\r\n"); 
		return -EINVAL; 
	} 
	
	/* 获取设备树中 spi0 节点的 cs-gpios 属性，得到 CS 所使用的 GPIO 编号 */ 
	ipsTftdev.cs_gpio = of_get_named_gpio(ipsTftdev.nd, "cs-gpios", 0); 
	if(ipsTftdev.cs_gpio < 0) { 
		printk("can't get cs-gpios"); 
		return -EINVAL; 
	} 
	/* 获取设备树中 testTFTRes 节点 */ 
    ipsTftdev.nd = of_find_node_by_path("/testTFTRes"); 
	if(ipsTftdev.nd == NULL) { 
		printk("res-gpio node not find!\r\n"); 
		return -EINVAL; 
    } 
	/* 获取设备树中 testTFTRes 节点的 gpios 属性，得到 RES 所使用的 GPIO 编号 */ 
    ipsTftdev.res_gpio = of_get_named_gpio(ipsTftdev.nd, "gpios", 0); 
    if(ipsTftdev.res_gpio < 0) { 
		printk("can't get res-gpio"); 
		return -EINVAL; 
	} 
	/* 获取设备树中 testTFTDc 节点 */ 
    ipsTftdev.nd = of_find_node_by_path("/testTFTDc"); 
	if(ipsTftdev.nd == NULL) { 
		printk("ipsDcgpio node not find!\r\n"); 
		return -EINVAL; 
    } 
	/* 获取设备树中 testTFTDc 节点的 gpios 属性，得到 RES 所使用的 GPIO 编号 */ 
    ipsTftdev.dc_gpio = of_get_named_gpio(ipsTftdev.nd, "gpios", 0); 
    if(ipsTftdev.dc_gpio < 0) { 
		printk("can't get ipsDc-gpio"); 
		return -EINVAL; 
	} 
	/* 设置GPIO1_IO20为输出，并且输出高电平 */ 
	ret = gpio_direction_output(ipsTftdev.cs_gpio, 1); 
	if(ret < 0) { 
		printk("can't set cs gpio!\r\n"); 
	} 
    ret = gpio_direction_output(ipsTftdev.res_gpio, 1); 
	if(ret < 0) { 
		printk("can't set res gpio!\r\n"); 
	} 
    ret = gpio_direction_output(ipsTftdev.dc_gpio, 1); 
	if(ret < 0) { 
		printk("can't set dc gpio!\r\n"); 
	} 
	/*初始化spi_device */ 
	spi->mode = SPI_MODE_2;	/*MODE0，CPOL=0，CPHA=0 */
	spi_setup(spi); 
	ipsTftdev.private_data = spi; /* 设置私有数据 */ 
	/* 初始化ipsTft内部寄存器 */ 
	ipsTft_reginit(&ipsTftdev); 
	return 0; 
} 
/* 
 * @description     : spi驱动的remove函数，移除spi驱动的时候此函数会执行 
 * @param - client 	: spi设备 
 * @return          : 0，成功;其他负值,失败 
 */ 
static int ipsTft_remove(struct spi_device *spi) 
{ 
	/* 删除设备 */ 
	cdev_del(&ipsTftdev.cdev); 
	unregister_chrdev_region(ipsTftdev.devid, ipsTft_CNT); 
	/* 注销掉类和设备 */ 
	device_destroy(ipsTftdev.class, ipsTftdev.devid); 
	class_destroy(ipsTftdev.class); 
	return 0; 
} 

/* 设备树匹配列表 */ 
static const struct of_device_id ipsTft_of_match[] = { 
	{ .compatible = "testspiTFT" }, 
	{ /* Sentinel */ } 
}; 
/* SPI驱动结构体 */ 
static struct spi_driver ipsTft_driver = { 
	.probe = ipsTft_probe, 
	.remove = ipsTft_remove, 
	.driver = { 
		   	.name = "ipsTft",                  /* 驱动名字，用于和设备匹配 */
		   	.of_match_table = ipsTft_of_match, /* 设备树匹配表 		 */ 
		   }, 
}; 
/* 
 * @description	: 驱动入口函数 
 * @param 		: 无 
 * @return 		: 无 
 */ 
static int __init ipsTft_init(void) 
{ 
	return spi_register_driver(&ipsTft_driver); 
} 
/* 
 * @description	: 驱动出口函数 
 * @param 		: 无 
 * @return 		: 无 
 */ 
static void __exit ipsTft_exit(void) 
{ 
	spi_unregister_driver(&ipsTft_driver); 
} 
module_init(ipsTft_init); 
module_exit(ipsTft_exit); 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("qlexcel"); 
