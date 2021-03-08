#!/bin/bash

input=$1
delay=$2
times=$3

if [ $# -lt 2 ]
then
    delay=3
fi

if [ $# -lt 3 ]
then
    times=10
fi

while [ $times -gt 0 ]
do
    echo $times": "$1
    sleep $delay
    times=$(($times - 1))
done