#!/bin/bash
# Suite de T6 (sans les matrices pour g/f : trop long) puis T2b.
cd "$(dirname "$0")"
while kill -0 9163 2>/dev/null; do sleep 20; done
PMAX=26 ./k4s -a alphas_lin.bin -n 30 -s 5000 -S 2 > res_T6_periodique26_lin.txt 2> log_T6_p26.txt
./k4x -a alphas_k2.bin -e pbcr -q qg.bin -n 10 -s 5000 -T 1 > res_T2_deux_mots.txt 2> log_T2_deux_mots.txt
