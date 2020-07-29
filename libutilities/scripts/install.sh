#!/bin/bash

if [ "$1" == "" ]; then
   mysql -p -D test < ../docs/create-funcs.sql
   exit
fi
mysql -p -D $1 < ../docs/create-funcs.sql



