 function slope_movement() %
%б���˶�
clear;
clc;
global location v0 alpha g;
options={'��ʼλ�ã����꣩','��ʼ�ٶ�v0' ,'����Ƕ�','�������ٶ�g',};
topic='seting';
lines=1;
def={'[0,0.150]','6.31','0','9.8'};
h=inputdlg(options,topic,lines,def);
location=eval(h{1});
v0=eval(h{2});
alpha=eval(h{3});
g=eval(h{4});
%***************************************************
a=location(1);
b=location(2);
alfa=alpha*pi/180;
tEnd=v0*sin(alfa)/g+((v0*sin(alfa)/g)^2+2*b/g)^0.5;%б��������˶�ʱ��
t=linspace(0,tEnd);
x=v0*cos(alfa)*t+a;%б�������ˮƽλ��
y=v0*sin(alfa)*t-0.5*g*t.^2+b;%б���������ֱλ��
plot(x,y);
hold on
plot(x(100),y(100),'o')

xlabel ˮƽ����/m
ylabel �߶�/m
title ����켣