#!/usr/bin/env bash
# E09 — exécution autonome. Tâches « CELLULE:langue », CELLULE ∈ {T1A14, T1B13, T0A14}, langue ∈ {en fr de it es la nl eo}.
# Règle pré-inscrite : réel + null seulement si contrôles ≥ 3/5. Une tâche par cœur.
# Usage (racine du dépôt) : bash dagapeyeff/experiments/E09_nihilist/run_E09.sh T1A14:fr T1B13:fr ...
set -u
ROOT=$(cd "$(dirname "$0")/../.." && pwd); OUT=$ROOT/experiments/E09_nihilist/logs_run; mkdir -p "$OUT"; BIN=$OUT/e09_solver
(cd "$ROOT/data" && sha256sum -c --quiet SHA256SUMS_models.txt) || { echo "empreintes invalides"; exit 1; }
gcc -O2 -o "$BIN" "$ROOT/tools/e03_solver.c" -lm || exit 1
CT=$ROOT/data/ciphertext_1939.txt
run_task(){ t=$1; cell=${t%%:*}; L=${t##*:}
  case $cell in T1A14) E="TRANSP=1 GEO=0 W=14";; T1B13) E="TRANSP=1 GEO=1 W=13";; T0A14) E="TRANSP=0 GEO=0 W=14";; *) echo "cellule inconnue $cell"; return;; esac
  case $L in en) k=1;; fr) k=2;; de) k=3;; it) k=4;; es) k=5;; la) k=6;; nl) k=7;; eo) k=8;; *) echo "langue inconnue $L"; return;; esac
  case $cell in T1A14) c=1;; T1B13) c=2;; T0A14) c=3;; esac
  S0=$((2000+c*100+k*10)); P="$E RESTARTS=24 ROUNDS=6 SIGIT=20000 SURR=1"; Q=$ROOT/data/models/qg_$L.bin; H=$ROOT/data/heldout/$L.txt; f="$OUT/${cell}_$L"
  env SEED=$S0 $P "$BIN" "$Q" "$CT" control 5 "$H" > "${f}_controls.out" 2>&1
  ok=$(grep '==>' "${f}_controls.out" | sed 's/==> \([0-9]*\)\/.*/\1/')
  if [ "${ok:-0}" -ge 3 ]; then env SEED=$((S0+1)) $P "$BIN" "$Q" "$CT" real > "${f}_real.out" 2>&1; env SEED=$((S0+2)) $P "$BIN" "$Q" "$CT" null 3 > "${f}_null.out" 2>&1
  else echo "contrôles ${ok:-0}/5 < 3 : non admissible, vrai chiffré NON lancé" > "${f}_real.out"; fi; }
export -f run_task; export ROOT OUT BIN CT
TASKS="$*"; [ -z "$TASKS" ] && { echo "préciser les tâches"; exit 1; }
echo "tâches : $TASKS ; cœurs : $(nproc) ; début $(date -u +%FT%TZ)"
printf '%s\n' $TASKS | xargs -P "$(nproc)" -I{} bash -c 'run_task {}'
{ echo "E09 — résumé ($(date -u +%FT%TZ))"; for t in $TASKS; do f="$OUT/${t%%:*}_${t##*:}"
  echo "$t | contrôles $(grep -h '==>' "${f}_controls.out" 2>/dev/null) | réel $(grep -h 'MEILLEUR' "${f}_real.out" 2>/dev/null | cut -c1-40) | null $(grep -h 'NULL' "${f}_null.out" 2>/dev/null | awk '{print $3}' | tr '\n' ' ')"; done; } | tee "$OUT/RESUME_$(date -u +%H%M%S).txt"
