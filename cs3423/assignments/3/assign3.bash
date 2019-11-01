#!/bin/bash

if [ ! -f $1 ]; then
    exit
fi

hour=`date "+%H" --date='Aug 16 2018 23:05'`
minutes=`date "+%M" --date='Aug 16 2018 23:05'`
gawk -f program.awk -v currHour=$hour -v currMin=$minutes $1
