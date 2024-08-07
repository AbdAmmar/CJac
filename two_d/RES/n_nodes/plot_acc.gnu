#!/bin/gnuplot

set terminal pngcairo size 1920,1080 enhanced font 'Arial,14'
set output 'acceleration_factor.png'

set grid
set title 'Acceleration Factor vs Number of Nodes' font 'Arial,16'

set xrange [0.5:15.5]
set xlabel 'Number of Nodes' font 'Arial,14'
set xtics 1

set ylabel 'Acceleration Factor' font 'Arial,14'

# 1 node
t1 = 23.367065

f(x) = x

plot 'RES_TURPAN' u 1:(t1/($2)) w lp t '' pt 7 ps 1.5 lw 2 lc rgb 'blue', \
     x                          w l  t '' dt 2        lw 2 lc rgb 'black'


