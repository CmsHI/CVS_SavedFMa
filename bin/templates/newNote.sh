#! /bin/bash -
if [ $# -eq 0 ]; then
   echo "usage: "
   echo "  $0 <note_type> <template_path> [additional_description_name]"
else
   newNote=$1_$3.txt
   #echo $newNote
   if [ -e $newNote ]; then
      echo file already exists!
      exit 1
   else
      cp $2 $newNote
      emacs -nw $newNote
   fi
fi

