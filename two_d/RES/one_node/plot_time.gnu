#!/bin/gnuplot

set terminal pngcairo size 1920,1080 enhanced font 'Arial,14'
set output 'wall_time.png'

set grid
set title 'Wall Time vs Number of Threads on One Node' font 'Arial,16'

set xrange [0.5:80.5]
set xlabel 'Number of Threads' font 'Arial,14'
set xtics (1, 10, 20, 30, 40, 50, 60, 70, 80)

set ylabel 'Wall Time (sec)' font 'Arial,14'
set logscale y

plot 'RES_TURPAN' u 1:2 w lp t '' pt 7 ps 1.5 lw 2 lc rgb 'blue'


