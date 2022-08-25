 function slope_movement() %
%斜抛运动
clear;
clc;
global location v0 alpha g;
options={'初始位置（坐标）','初始速度v0' ,'抛射角度','重力加速度g',};
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
tEnd=v0*sin(alfa)/g+((v0*sin(alfa)/g)^2+2*b/g)^0.5;%斜抛物体的运动时间
t=linspace(0,tEnd);
x=v0*cos(alfa)*t+a;%斜抛物体的水平位移
y=v0*sin(alfa)*t-0.5*g*t.^2+b;%斜抛物体的竖直位移
plot(x,y);
hold on
plot(x(100),y(100),'o')

xlabel 水平距离/m
ylabel 高度/m
title 抛体轨迹