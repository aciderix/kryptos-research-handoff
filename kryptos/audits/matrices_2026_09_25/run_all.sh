#!/bin/sh
# balayages sur les alphabets en matrice : K4 puis témoin « K4 mélangé » (graine 77), 4 tranches en parallèle
QG=/tmp/claude-0/-home-user-kryptos-research-handoff/001fee97-849c-56c2-808b-4e071a32a106/scratchpad/qg.bin
for prog in kwautokey kwcak kwsweep; do
  for seed in 0 77; do
    for c in 0 1 2 3; do
      if [ $prog = kwsweep ]; then
        if [ $seed = 0 ]; then ONLYSTD=1 ./kwsweep chunk_0$c > out_${prog}_${seed}_$c.txt & else ONLYSTD=1 SHUF=1 ./kwsweep chunk_0$c $seed > out_${prog}_${seed}_$c.txt & fi
      else
        if [ $seed = 0 ]; then ONLYSTD=1 ./$prog $QG chunk_0$c > out_${prog}_${seed}_$c.txt & else ONLYSTD=1 SHUF=1 ./$prog $QG chunk_0$c $seed > out_${prog}_${seed}_$c.txt & fi
      fi
    done
    wait
    echo "$prog $seed fini $(date)"
  done
done
