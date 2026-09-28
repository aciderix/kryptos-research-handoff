#!/usr/bin/env bash
# E15 — exécution autonome. Tâches : ctrl:N:SEED (N contrôles), null:N:SEED, real:SEED. Une tâche par cœur.
# Usage (racine du dépôt) : bash dagapeyeff/experiments/E15_thirteen_classes_double/run_E15.sh ctrl:3:1501 ctrl:3:1502 ...
set -u
ROOT=$(cd "$(dirname "$0")/../.." && pwd); OUT=$ROOT/experiments/E15_thirteen_classes_double/logs_run; mkdir -p "$OUT"; BIN=$OUT/e15_scan
(cd "$ROOT/data" && sha256sum -c --quiet SHA256SUMS_models.txt) || { echo "empreintes invalides"; exit 1; }
gcc -O3 -march=native -o "$BIN" "$ROOT/tools/e04_scan.c" -lm || exit 1
CT=$ROOT/data/ciphertext_1939.txt; Q=$ROOT/data/models/qg_en.bin; H=$ROOT/data/heldout/en.txt
P="GEO=1 FAM=D W1MAX=7 W2MAX=7 TOP=10 PAIR=1 PR_REST=6 PR_ITERS=150000"
run_task(){ IFS=: read -r kind a b <<< "$1"
  case $kind in
    ctrl) env $P CLS13=1 SEED=$b "$BIN" "$Q" "$CT" control $a "$H" > "$OUT/ctrl_seed$b.out" 2>&1;;
    null) env $P SEED=$b "$BIN" "$Q" "$CT" null $a > "$OUT/null_seed$b.out" 2>&1;;
    real) env $P SEED=$a "$BIN" "$Q" "$CT" real > "$OUT/real_seed$a.out" 2>&1;;
  esac; }
export -f run_task; export ROOT OUT BIN CT Q H P
echo "tâches : $* ; cœurs : $(nproc) ; début $(date -u +%FT%TZ)"
printf '%s\n' "$@" | xargs -P "$(nproc)" -I{} bash -c 'run_task {}'
echo "fin $(date -u +%FT%TZ)"; grep -h -E "CTRL|NULL|MEILLEUR|==>" "$OUT"/*.out | tee "$OUT/RESUME.txt"
