#!/bin/bash

. /etc/default/pbs_licenses

server=$SERVER_NAME
metric=dynamic_resources

PATH=$PATH:/usr/sbin

ansys=`$ROOT/ansyslic | tr -d '\n'`;
matlab=`$ROOT/matlablic | tr -d '\n'`;
fluent=`$ROOT/fluentlic | tr -d '\n'`;
marc=`$ROOT/marclic | tr -d '\n'`;
marcn=`$ROOT/marcnlic | tr -d '\n'`;
maple10=`$ROOT/maple10lic | tr -d '\n'`;
maple10p=`$ROOT/maple10plic | tr -d '\n'`;
maple11=`$ROOT/maple11lic | tr -d '\n'`;

#for server in wagap.cerit-sc.cz
#do

update_cache $server ansys  $metric $ansys
#update_cache $server matlab $metric $matlab
update_cache $server fluent $metric $fluent
update_cache $server marc   $metric $marc
update_cache $server marcn  $metric $marcn
update_cache $server maple10  $metric $maple10
update_cache $server maple10p $metric $maple10p
update_cache $server maple11  $metric $maple11

$ROOT/matlablic.detail+meta | \
	while read feature free ;do 
		if [ "$feature" = matlab_MATLAB ];then
			feature=matlab
		fi
		update_cache $server "$feature" $metric "$free"
	done &

$ROOT/sokarlic.detail+meta | \
	while read feature free ;do 
		update_cache $server "$feature" $metric "$free"
        done &

#done
wait
