close all
clear all
clc

load('torqueRegData')


qq=q.signals.values;
qqd=qd.signals.values;
time=1:length(qq);

plot(time,qq);
hold on
plot(time,qqd);

e=qqd-qq;
de=[0; diff(e)]/0.01;

torque=900*e+15*de;
torque=omejitev(torque,900);

plot(time,qq,'r');
hold on
plot(time,qqd);
plot(time,70+10*[0; diff(qq)],'-black');
plot([0 9000],[70 70],'.black')

plot(time,torque/100,'m')
plot(time,e,'g')
plot(time,de/50,'c')


(Mm + Kt*a + (Fz^2*Kz^2 + 2*Fz*J*Kz*a*s + 2*Fz*Kt*Kz*a - 2*Fz*Kz*Mm + J^2*a^2*s^2 + 2*J*Kt*a^2*s - 2*J*Mm*a*s - 4*Kl*J*s + Kt^2*a^2 - 2*Kt*Mm*a - 4*Kl*Kt + Mm^2)^(1/2) - Fz*Kz + J*a*s)/(2*J*s^2 + 2*Kt*s)
(Mm + Kt*a - (Fz^2*Kz^2 + 2*Fz*J*Kz*a*s + 2*Fz*Kt*Kz*a - 2*Fz*Kz*Mm + J^2*a^2*s^2 + 2*J*Kt*a^2*s - 2*J*Mm*a*s - 4*Kl*J*s + Kt^2*a^2 - 2*Kt*Mm*a - 4*Kl*Kt + Mm^2)^(1/2) - Fz*Kz + J*a*s)/(2*J*s^2 + 2*Kt*s)
 

