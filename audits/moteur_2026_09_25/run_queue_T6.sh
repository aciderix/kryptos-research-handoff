#!/bin/bash
# T6 : familles indécidables de la base 2 rejouées avec alphabets à mot-clé (après run_queue.sh).
cd "$(dirname "$0")"
while pgrep -f run_queue.sh >/dev/null; do sleep 30; done
./k4x -a alphas_lin.bin -e gf -n 30 -s 5000 -T 1 > res_T6_lin.txt 2> log_T6_lin.txt
./k4x -a alphas_base.bin -e gf -n 30 -s 5000 -T 1 > res_T6_mat.txt 2> log_T6_mat.txt
PMAX=26 ./k4s -a alphas_lin.bin -n 30 -s 5000 -S 2 > res_T6_periodique26_lin.txt 2> log_T6_p26.txt
