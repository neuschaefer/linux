#!/bin/sh
# wpcm-next.sh - create the WPCM450 -next branch

git merge --commit --no-ff \
	b4/wpcm-clk \
	b4/wpcm-kconfig \
	wpcm-eth
