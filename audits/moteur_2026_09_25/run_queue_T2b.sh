#!/bin/bash
# T2b : alphabets à deux mots-clés (mot thématique + mot du dictionnaire, dans les deux ordres), après les autres files.
cd "$(dirname "$0")"
while pgrep -f "run_queue.sh|run_queue_T6.sh" >/dev/null; do sleep 30; done
./k4x -a alphas_k2.bin -e pbcr -q qg.bin -n 10 -s 5000 -T 1 > res_T2_deux_mots.txt 2> log_T2_deux_mots.txt
