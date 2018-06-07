#!/bin/bash

. /etc/default/pbs_licenses

servers="$SERVER_NAME"
metric=dynamic_resources

PATH=$PATH:/usr/sbin

licname="$1"

case $licname in
ansys|fluent|marc|marcn|maple10|maple10p|maple11)
	count=`$ROOT/${licname}lic | tr -d '\n'`;
	for server in $servers; do update_cache $server $licname  $metric $count 2>/dev/null; done
	;;
matlab|sokar|geneious|clcgenomics)
	tempfile=`mktemp`
	for server in $servers
	do
		PBS_SERVER=$server $ROOT/${licname}lic.detail+meta > $tempfile
		while read feature free ;do 
		 	update_cache $server "$feature" $metric "$free" 2>/dev/null
		done < $tempfile
	done
	rm $tempfile
	;;
maple)
	tempfile=`mktemp`
	$ROOT/${licname}lic.detail > $tempfile
	for server in $servers
	do

		while read feature free ;do 
		 	update_cache $server "$feature" $metric "$free" 2>/dev/null
		done < $tempfile
	done
	rm $tempfile
	;;
esac
