#!/bin/sh
for i in *.html
do  
  sed 's/_8c/.c/g' $i > $i.tmp
  mv $i.tmp $i
done