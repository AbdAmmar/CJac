#!/bin/gnuplot

set terminal pngcairo size 1920,1080 enhanced font 'Arial,14'
set output 'wall_time.png'

set grid
set title 'Wall Time vs Number of Threads on One Node' font 'Arial,16'

set xrange [1:80]
set xlabel 'Number of Threads' font 'Arial,14'


set ylabel 'Wall Time (sec)' font 'Arial,14'
set logscale y

plot 'RES' using 1:2 with lp t '' pt 7 ps 1.5 lw 2 lc rgb 'blue'


