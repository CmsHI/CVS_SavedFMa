#! /bin/bash -
newREADME=README_$1.txt
#echo $newREADME
if [ -e $newREADME ]; then
#if [ -e README_.txt ]; then
   echo file already exists!
   exit 1
else
   cp $HOME/bin/templates/README_new.txt $newREADME
   emacs -nw $newREADME
fi

