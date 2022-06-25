clc
clear all
close all

x = 1;
v = 0;
dt = 0.01;
k = 0.5;
c = 0.01;

pos(1) = x;
time(1) = 0;
for i = 2:1000/dt
    a = -k*x*dt -c*v*dt;
    v_new = v + a*dt;
    x_new = x + v*dt;
    v = v_new;
    x = x_new;
    pos(i) = x;
    time(i) = time(i-1) + dt; 
endfor

plot(time,pos);
title(sprintf("dt = %i e k = %i com c = %i",dt,k,c));
ylabel("Posição (cm)");
xlabel("Tempo (s)");