from fpioa_manager import fm
from machine import UART
import sensor, image, lcd, time
import KPU as kpu
import gc, sys, time
fm.register(9, fm.fpioa.UART1_TX, force=True)
fm.register(10, fm.fpioa.UART1_RX, force=True)
uart_A = UART(UART.UART1, 115200, 8, 1, 0, timeout=1000, read_buf_len=4096)
none=14
time.sleep_ms(50)
def lcd_show_except(e):
	import uio
	err_str = uio.StringIO()
	sys.print_exception(e, err_str)
	err_str = err_str.getvalue()
	img = image.Image(size=(224,224))
	img.draw_string(0, 10, err_str, scale=1, color=(0xff,0x00,0x00))
	lcd.display(img)
def main(labels = None, model_addr="/sd/m.kmodel", sensor_window=(224, 224), lcd_rotation=0, sensor_hmirror=False, sensor_vflip=False):
	sensor.reset()
	sensor.set_pixformat(sensor.RGB565)
	sensor.set_framesize(sensor.QVGA)
	sensor.set_windowing(sensor_window)
	sensor.set_hmirror(sensor_hmirror)
	sensor.set_vflip(sensor_vflip)
	sensor.run(1)
	lcd.init(type=1)
	lcd.rotation(lcd_rotation)
	lcd.clear(lcd.WHITE)
	if not labels:
		with open('labels.txt','r') as f:
			exec(f.read())
	if not labels:
		print("no labels.txt")
		img = image.Image(size=(320, 240))
		img.draw_string(90, 110, "no labels.txt", color=(255, 0, 0), scale=2)
		lcd.display(img)
		return 1
	try:
		img = image.Image("startup.jpg")
		lcd.display(img)
	except Exception:
		img = image.Image(size=(320, 240))
		img.draw_string(90, 110, "loading model...", color=(255, 255, 255), scale=2)
		lcd.display(img)
	try:
		a=0
		task = None
		task = kpu.load(model_addr)
		while(True):
			img = sensor.snapshot()
			fmap = kpu.forward(task, img)
			plist=fmap[:]
			pmax=max(plist)
			if pmax<0.60:
				a=a+1
				img = image.Image('/sd/'+str(a)+'.jpg')
				lcd.display(img)
				time.sleep_ms(50)
				uart_A.write(none.to_bytes(1,'little'))
				time.sleep_ms(1500)
			if pmax>0.60:
				max_index=plist.index(pmax)
				lcd.display(img)
				time.sleep_ms(50)
				uart_A.write(max_index.to_bytes(1,'little'))
				time.sleep_ms(1500)
			if a==10:
				a=1
	except Exception as e:
		raise e
	finally:
		if not task is None:
			kpu.deinit(task)
if __name__ == "__main__":
	try:
		labels = ["c_bottle", "c_can", "h_battery", "k_apple", "k_banana", "k_cai", "k_mango", "k_orange", "k_pear", "k_potato", "k_radish", "k_tomato", "o_smoke", "o_zwtc"]
		main(labels=labels, model_addr=0x300000)
	except Exception as e:
		sys.print_exception(e)
		lcd_show_except(e)
	finally:
		gc.collect()
uart_A.deinit()
del uart_A
