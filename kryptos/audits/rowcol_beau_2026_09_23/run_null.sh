#!/bin/bash
cd "$(dirname "$0")"
: > null_cpsat.jsonl
for w in $(cat sat_widths.txt); do for s in 1 2 3 4 5 6 7 8 9 10; do echo "$w $s"; done; done | \
  xargs -P 3 -n 2 sh -c 'timeout 300 python3 rowcol_cpsat.py beau $0 $1 || echo "{\"mode\": \"beau\", \"w\": $0, \"ct\": \"rand$1\", \"sat\": \"TIMEOUT\"}"' >> null_cpsat.jsonl
echo DONE >> null_cpsat.jsonl
