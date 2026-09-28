#!/usr/bin/env bash
# E06 — exécution autonome des cellules « autres langues » (géométrie B) et « anglais, géométrie A ».
# Applique la règle pré-inscrite : le vrai chiffré (et son null) n'est lancé pour une tâche que si ses contrôles
# atteignent le seuil (≥ 3/5). Utilise tous les cœurs (une tâche par cœur).
# Usage (depuis la racine du dépôt, branche claude/ecstatic-volta-cn51vq) :
#   bash dagapeyeff/experiments/E06_parity_read/run_E06.sh            # toutes les tâches
#   bash dagapeyeff/experiments/E06_parity_read/run_E06.sh fr de      # seulement certaines
# Résultats : dagapeyeff/experiments/E06_parity_read/logs_run/ ; résumé : logs_run/RESUME.txt
set -u
ROOT=$(cd "$(dirname "$0")/../.." && pwd)          # .../dagapeyeff
OUT=$ROOT/experiments/E06_parity_read/logs_run; mkdir -p "$OUT"
BIN=$OUT/e06_scan
(cd "$ROOT/data" && sha256sum -c --quiet SHA256SUMS_models.txt) || { echo "empreintes des modèles invalides"; exit 1; }
gcc -O3 -march=native -o "$BIN" "$ROOT/tools/e04_scan.c" -lm || exit 1
CT=$ROOT/data/ciphertext_1939.txt
TASKS=${*:-"fr de it es la nl enA"}
P="FAM=P TOP=50 REST2=8 ITER2=40000"

run_task(){ # $1 = tâche
  t=$1; G=1; L=$t
  case $t in fr) S0=810;; de) S0=820;; it) S0=830;; es) S0=840;; la) S0=850;; nl) S0=860;; enA) S0=870; L=en; G=0;; *) echo "tâche inconnue $t"; return;; esac
  Q=$ROOT/data/models/qg_$L.bin; H=$ROOT/data/heldout/$L.txt
  env SEED=$S0 GEO=$G $P "$BIN" "$Q" "$CT" control 5 "$H" > "$OUT/${t}_controls.out" 2>&1
  ok=$(grep '==>' "$OUT/${t}_controls.out" | sed 's/==> \([0-9]*\)\/.*/\1/')
  if [ "${ok:-0}" -ge 3 ]; then
    env SEED=$((S0+1)) GEO=$G $P "$BIN" "$Q" "$CT" real   > "$OUT/${t}_real.out" 2>&1
    env SEED=$((S0+2)) GEO=$G $P "$BIN" "$Q" "$CT" null 3 > "$OUT/${t}_null.out" 2>&1
  else echo "contrôles ${ok:-0}/5 < 3 : tâche non admissible, vrai chiffré NON lancé" > "$OUT/${t}_real.out"; fi
}
export -f run_task; export ROOT OUT BIN CT P
echo "tâches : $TASKS ; cœurs : $(nproc) ; début $(date -u +%FT%TZ)"
printf '%s\n' $TASKS | xargs -P "$(nproc)" -I{} bash -c 'run_task {}'
{ echo "E06 — résumé ($(date -u +%FT%TZ), $(nproc) cœurs)"
  for t in $TASKS; do
    c=$(grep -h '==>' "$OUT/${t}_controls.out" 2>/dev/null); r=$(grep -h 'MEILLEUR' "$OUT/${t}_real.out" 2>/dev/null | cut -c1-60)
    n=$(grep -h 'NULL' "$OUT/${t}_null.out" 2>/dev/null | awk '{print $3}' | tr '\n' ' ')
    echo "$t | contrôles $c | réel $r | null $n"; done; } | tee "$OUT/RESUME.txt"
