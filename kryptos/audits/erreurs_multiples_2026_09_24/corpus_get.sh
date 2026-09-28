#!/bin/sh
# Corpus anglais (projet Gutenberg) pour les bigrammes et les quadrigrammes ; à télécharger dans $1.
mkdir -p "$1"; cd "$1" || exit 1
for id in 1342 1661 2701 98 36 84 1260 174 345 768 1400 2600 4300 5200 3207 6130 1184 219 76 74 120 244 2852 1952 30254 16328; do
  curl -sS -m 30 -o pg$id.txt https://www.gutenberg.org/cache/epub/$id/pg$id.txt
done
