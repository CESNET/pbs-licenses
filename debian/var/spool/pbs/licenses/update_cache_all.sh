#!/bin/bash

. /etc/default/pbs_licenses

for i in $ALL_SINGLE; do
	$ROOT/update_cache_single.sh $i &> /dev/null
done

for i in $ADD_ON; do
	$ROOT/$i &> /dev/null
done
