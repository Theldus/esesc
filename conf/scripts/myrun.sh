#!/bin/bash
rm esesc_L2* 2> /dev/null
l2assoc=$(cat simu.conf | grep -i '\[PrivL2\]'  -A 9  | grep "assoc"      | sed 's/[^0-9*]//g')
l2polic=$(cat simu.conf | grep -i '\[PrivL2\]'  -A 12 | grep "replPolicy" | cut -d"'" -f2)
l3polic=$(cat simu.conf | grep -i '\[L3Cache\]' -A 12 | grep "replPolicy" | cut -d"'" -f2)
date=$(date '+%Y%m%d-%H%M%S')

export REPORTFILE="L2_[P_"$l2polic"_A_"$l2assoc"]_L3_[P_"$l3polic]"_"$date
time ./esesc

cat esesc_* |\
 grep -P "L[23]\(0\)" |\
 grep -P "readHit|readMiss|writeHit|writeMiss" |\
 grep -v "State" > "$REPORTFILE.output"

rm esesc_L2* 2> /dev/null
