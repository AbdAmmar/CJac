#!/bin/gnuplot

set terminal pngcairo size 1920,1080 enhanced font 'Arial,14'
set output 'wall_time.png'

set grid
set title 'Wall Time vs Number of Nodes' font 'Arial,16'

set xrange [0.5:15.5]
set xlabel 'Number of Nodes' font 'Arial,14'
set xtics 1

set yrange [1:31]
set ylabel 'Wall Time (sec)' font 'Arial,14'
set logscale y

plot 'RES_TURPAN' u 1:2 w lp t '' pt 7 ps 1.5 lw 2 lc rgb 'blue'


