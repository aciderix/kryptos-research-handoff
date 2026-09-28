#!/usr/bin/env bash
# E07 — exécution autonome (espéranto). Tâches : EB EA IB IA DB (famille + géométrie). Règle pré-inscrite appliquée :
# le vrai chiffré et son null ne sont lancés que si les contrôles atteignent le seuil (≥ 6/10 ou ≥ 3/5).
# Usage (racine du dépôt) : bash dagapeyeff/experiments/E07_esperanto/run_E07.sh [tâches…]   (défaut : toutes)
set -u
ROOT=$(cd "$(dirname "$0")/../.." && pwd); OUT=$ROOT/experiments/E07_esperanto/logs_run; mkdir -p "$OUT"; BIN=$OUT/e07_scan
(cd "$ROOT/data" && sha256sum -c --quiet SHA256SUMS_models.txt) || { echo "empreintes invalides"; exit 1; }
gcc -O3 -march=native -o "$BIN" "$ROOT/tools/e04_scan.c" -lm || exit 1
CT=$ROOT/data/ciphertext_1939.txt; Q=$ROOT/data/models/qg_eo.bin; H=$ROOT/data/heldout/eo.txt
TASKS=${*:-"EB EA IB IA DB"}
run_task(){ t=$1
  case $t in
    EB) F="FAM=E WMIN=2 WMAX=11 TOP=50"; G=1; NC=10; TH=6; NN=5; S0=1010;;
    EA) F="FAM=E WMIN=2 WMAX=11 TOP=50"; G=0; NC=5;  TH=3; NN=5; S0=1020;;
    IB) F="FAM=I WMIN=2 WMAX=11 TOP=50"; G=1; NC=10; TH=6; NN=5; S0=1030;;
    IA) F="FAM=I WMIN=2 WMAX=11 TOP=50"; G=0; NC=5;  TH=3; NN=5; S0=1040;;
    DB) F="FAM=D W1MAX=7 W2MAX=7 TOP=20"; G=1; NC=5; TH=3; NN=3; S0=1050;;
    *) echo "tâche inconnue $t"; return;; esac
  P="$F REST2=8 ITER2=40000 GEO=$G"
  env SEED=$S0 $P "$BIN" "$Q" "$CT" control $NC "$H" > "$OUT/${t}_controls.out" 2>&1
  ok=$(grep '==>' "$OUT/${t}_controls.out" | sed 's/==> \([0-9]*\)\/.*/\1/')
  if [ "${ok:-0}" -ge $TH ]; then
    env SEED=$((S0+1)) $P "$BIN" "$Q" "$CT" real > "$OUT/${t}_real.out" 2>&1
    env SEED=$((S0+2)) $P "$BIN" "$Q" "$CT" null $NN > "$OUT/${t}_null.out" 2>&1
  else echo "contrôles ${ok:-0}/$NC < $TH : non admissible, vrai chiffré NON lancé" > "$OUT/${t}_real.out"; fi; }
export -f run_task; export ROOT OUT BIN CT Q H
echo "tâches : $TASKS ; cœurs : $(nproc) ; début $(date -u +%FT%TZ)"
printf '%s\n' $TASKS | xargs -P "$(nproc)" -I{} bash -c 'run_task {}'
{ echo "E07 — résumé ($(date -u +%FT%TZ))"; for t in $TASKS; do
  echo "$t | contrôles $(grep -h '==>' "$OUT/${t}_controls.out" 2>/dev/null) | réel $(grep -h 'MEILLEUR' "$OUT/${t}_real.out" 2>/dev/null | cut -c1-70) | null $(grep -h 'NULL' "$OUT/${t}_null.out" 2>/dev/null | awk '{print $3}' | tr '\n' ' ')"; done; } | tee "$OUT/RESUME.txt"
