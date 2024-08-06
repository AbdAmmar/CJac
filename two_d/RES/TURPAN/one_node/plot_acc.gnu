#!/bin/gnuplot

set terminal pngcairo size 1920,1080 enhanced font 'Arial,14'
set output 'acceleration_factor.png'

set grid
set title 'Acceleration Factor vs Number of Threads on One Node' font 'Arial,16'

set xrange [1:80]
set xlabel 'Number of Threads' font 'Arial,14'


set ylabel 'Acceleration Factor' font 'Arial,14'
set logscale y

# 1 thread
t1 = 866.485884

plot 'RES' using 1:(t1/($2)) with lp t '' pt 7 ps 1.5 lw 2 lc rgb 'blue'


