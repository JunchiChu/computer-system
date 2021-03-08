#!/bin/bash

sigint_handler()
{
   echo ""
   echo "GOTCHYA (SIGINT)"
   exit
}
#sigstp_handler() {
#   echo ""
#   echo "GOTCHYA (SIGSTP)"
#   exit
#}
sigquit_handler() {
    echo ""
    echo "GOTCHYA (SIGQUIT)"
    exit
}
trap sigint_handler SIGINT
#trap sigstp_handler TSTP
trap sigquit_handler SIGQUIT
# Loop forever, reporting life each second
SEC=0
while true ; do
	sleep 1
	SEC=$((SEC+1))
	echo "PID# $$[$SEC]"
done

# We never get here.
exit 0
