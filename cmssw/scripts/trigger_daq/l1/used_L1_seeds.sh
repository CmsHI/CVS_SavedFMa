#!/bin/bash
if [ $# -eq 0 ]; then
   echo "$0"
   echo "usage:"
   echo "   $0 <cfg_file_that_uses_L1_seeds>"
   echo "output:"
   echo "   stdout: a (sorted, unique) list of the L1 bits used"
   echo "   in the cfg file"
else
   grep -o L1_[[:alnum:]]* $1 | grep -v L1Extra | sort | uniq
fi

