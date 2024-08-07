#!/bin/gnuplot

set terminal pngcairo size 1920,1080 enhanced font 'Arial,14'
set output 'acceleration_factor.png'

set grid
set title 'Acceleration Factor vs Number of Threads on One Node' font 'Arial,16'

set xrange [0.5:80.5]
set xlabel 'Number of Threads' font 'Arial,14'
#set xtics 5
set xtics (1, 10, 20, 30, 40, 50, 60, 70, 80)

set yrange [1:81]
set ylabel 'Acceleration Factor' font 'Arial,14'

# 1 thread
t1 = 866.485884

plot 'RES_TURPAN' u 1:(t1/($2)) w lp t '' pt 7 ps 1.5 lw 2 lc rgb 'blue', \
     x                          w l  t '' dt 2        lw 2 lc rgb 'black'


