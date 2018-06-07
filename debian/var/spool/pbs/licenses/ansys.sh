#!/bin/sh

. /etc/default/pbs_licenses

server=$SERVER_NAME

$ROOT/ansys.detail | while read x; do 

$(echo $x | /usr/bin/awk -v server=$server '{ print "/usr/bin/update_cache "server" "$1" dynamic_resources "$2}'); 

done
