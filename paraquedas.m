clc
clear all
close all

x = 10;
v = 0;
dt = 0.01;
k = 1.5;
#g = 9.80665;
g = 10;

line(1) = 0;
pos(1) = x;
vel(1) = v;
acc(1) = -g + k*power(v,2);
time(1) = 0;
for i = 2:5/dt
    a = -g + k*power(v,2);
    v = v + a*dt;
    x = x + v*dt;
    
    acc(i) = a;
    vel(i) = v;
    pos(i) = x;
    line(i) = 0;
    time(i) = time(i-1) + dt; 
endfor

plot(time,pos);
hold on 
plot(time,vel);
hold on 
plot(time,acc);
hold on 
plot(time,line,'k');

legend("posicao","velocidade","aceleracao");
title(sprintf("dt = %i e k = %i",dt,k));
ylabel("Posicao (m)");
xlabel("Tempo (s)");