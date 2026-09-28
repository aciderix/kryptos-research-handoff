#!/bin/bash
# File d'attente des balayages (T2, T3, T4-QIV), lancée après T1 et TABP.
cd "$(dirname "$0")"
while pgrep -x k4tp >/dev/null; do sleep 20; done
# T3 : modèle d'erreur de Sanborn, clé périodique p = 1..14
./k4s -a alphas_lin.bin -n 30 -s 5000 > res_T3_sanborn_lin.txt 2> log_T3_lin.txt
./k4s -a alphas_base.bin -n 30 -s 5000 > res_T3_sanborn_mat.txt 2> log_T3_mat.txt
# T4 : Quagmire IV (deux alphabets à mot-clé), familles p, b, t
./k4x -a th_all.bin -2 th_all.bin -e pbt -n 30 -s 5000 -T 2 > res_T4_qiv_themes.txt 2> log_T4_qiv_th.txt
./k4x -a alphas_lin.bin -2 th_coeur.bin -e pbt -n 10 -s 5000 -T 2 > res_T4_qiv_dico.txt 2> log_T4_qiv_dico.txt
# T2 : dictionnaire élargi (430 000 mots : noms propres, lieux, thèmes)
while pgrep -x k4x | grep -qv $$; do sleep 20; done
./k4x -a alphas_dico.bin -e pbtacmr -q qg.bin -n 30 -s 5000 -T 2 > res_T2_dico.txt 2> log_T2_dico.txt
./k4s -a alphas_dico.bin -n 30 -s 5000 > res_T3_sanborn_dico.txt 2> log_T3_dico.txt
