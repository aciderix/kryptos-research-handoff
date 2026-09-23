#!/bin/bash
cd "$(dirname "$0")"
: > results_cpsat.jsonl
seq 2 48 | xargs -P 3 -I{} sh -c 'timeout 700 python3 rowcol_cpsat.py beau {} || echo "{\"mode\": \"beau\", \"w\": {}, \"sat\": \"TIMEOUT\"}"' >> results_cpsat.jsonl
echo DONE >> results_cpsat.jsonl
