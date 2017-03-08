#!/bin/sh
time for i in {1..32} ; do echo "$i $1 $2 $3" ; $1 $2 $3 ; done
