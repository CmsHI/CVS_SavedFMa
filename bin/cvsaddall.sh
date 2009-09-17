#!/bin/bash -

#
# script to recursively add subdirectories/files
# - cf
#   * http://vafer.org/blog/20050107005746
#

if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  cvsaddall <dir>"
   exit 1
fi

targetdir=$1
find $targetdir -type d -print | grep -v CVS | xargs cvs add
find $targetdir -type f -print | grep -v CVS | xargs cvs add

