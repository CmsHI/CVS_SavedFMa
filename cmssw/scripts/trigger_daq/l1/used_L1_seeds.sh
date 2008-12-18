#!/bin/bash
echo $#
if [ $# -eq 0 ]; then
   echo "$0 usage:"
   echo "   $0 <cfg_file_that_uses_L1_seeds>"
else
   grep -o L1_[[:alnum:]]* $1 | grep -v L1Extra | sort | uniq
fi

