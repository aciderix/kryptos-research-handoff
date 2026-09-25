#!/bin/bash
# T6 (g, f ; linéaires puis matrices), période ≤ 26 (k4s), puis T2b (deux mots-clés).
cd "$(dirname "$0")"
./k4x -a alphas_lin.bin -e gf -n 30 -s 5000 -T 1 > res_T6_lin.txt 2> log_T6_lin.txt
PMAX=26 ./k4s -a alphas_lin.bin -n 30 -s 5000 -S 2 > res_T6_periodique26_lin.txt 2> log_T6_p26.txt
./k4x -a alphas_k2.bin -e pbcr -q qg.bin -n 10 -s 5000 -T 1 > res_T2_deux_mots.txt 2> log_T2_deux_mots.txt
./k4x -a alphas_base.bin -e gf -n 30 -s 5000 -T 1 > res_T6_mat.txt 2> log_T6_mat.txt
