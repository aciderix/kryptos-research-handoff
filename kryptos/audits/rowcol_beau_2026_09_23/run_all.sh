#!/bin/bash
# one width per process, 3 in parallel, wall-clock cap 900 s per width; undecided widths are logged as TIMEOUT
cd "$(dirname "$0")"
: > results.jsonl
seq 2 48 | xargs -P 3 -I{} bash -c 'r=$(timeout 900 python3 one_width.py {}); if [ -n "$r" ]; then echo "$r"; else echo "{\"w\": {}, \"K4\": \"TIMEOUT_900s\"}"; fi >> results.jsonl'
echo DONE >> results.jsonl
