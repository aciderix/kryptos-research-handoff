#!/bin/sh
for s in $@; do THEME=themes.txt ./kwrunkey /tmp/claude-0/-home-user-kryptos-research-handoff/001fee97-849c-56c2-808b-4e071a32a106/scratchpad/qg.bin words.txt $s | sed -n 2p | sed "s/^/graine $s : /"; done
