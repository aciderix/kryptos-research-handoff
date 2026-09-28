#!/bin/sh
# Reconstruit un corpus anglais ~7M chars (domaine public, Gutenberg) pour qg_big.bin.
# Puis: cat corpus/*.txt | tr '[:lower:]' '[:upper:]' | tr -cd 'A-Z' > corpus_big.txt
#       cc -O2 -o build_qg build_qg.c -lm && ./build_qg corpus_big.txt qg_big.bin
mkdir -p corpus; cd corpus
for id in 2600 1342 2701 84 98 1400 730 76 2542; do
  curl -sS --max-time 60 -o "$id.txt" "https://www.gutenberg.org/files/$id/$id-0.txt"
done
