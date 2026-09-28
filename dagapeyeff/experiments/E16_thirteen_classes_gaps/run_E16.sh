#!/usr/bin/env bash
# E16 — exécution. Tâches CELL:MODE:N:SEED (MODE = control|null|s1ctrl|s1null|real ; N ignoré pour real).
# Usage (racine d'un clone) : bash dagapeyeff/experiments/E16_thirteen_classes_gaps/run_E16.sh R:control:4:1601 ...
set -u
ROOT=$(cd "$(dirname "$0")/../.." && pwd); OUT=$ROOT/experiments/E16_thirteen_classes_gaps/logs; mkdir -p "$OUT"; BIN=$OUT/../e16_bin
(cd "$ROOT/data" && sha256sum -c --quiet SHA256SUMS_models.txt) || { echo "empreintes invalides"; exit 1; }
gcc -O3 -march=native -o "$BIN" "$ROOT/tools/e16_h13.c" -lm || exit 1
CT=$ROOT/data/ciphertext_1939.txt; Q=$ROOT/data/models/qg_en.bin; H=$ROOT/data/heldout/en.txt
run_task(){ IFS=: read -r cell mode n seed <<< "$1"
  SEED=$seed "$BIN" "$Q" "$CT" "$H" "$mode" "$cell" "$n" > "$OUT/${cell}_${mode}_seed$seed.out" 2>&1; }
export -f run_task; export OUT BIN CT Q H
echo "tâches : $* ; cœurs : $(nproc) ; début $(date -u +%FT%TZ)"
printf '%s\n' "$@" | xargs -P "$(nproc)" -I{} bash -c 'run_task {}'
echo "fin $(date -u +%FT%TZ)"
