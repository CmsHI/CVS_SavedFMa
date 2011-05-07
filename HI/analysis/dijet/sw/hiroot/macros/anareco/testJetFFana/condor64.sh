#!/bin/bash

dir=`pwd`
ldir=~/condor
outfile=$1-output-`date +"%s"`

subfile=subfile_$outfile

echo $subfile
mkdir -p $ldir
mkdir -p /tmp/frankma

cat > $subfile <<EOF

Universe     = vanilla

Notification = Error
Executable   = $dir/$1
Arguments    = $2 $3 $4 $5 $6 $7 $8 $9
Requirements = (Mips > 900) && (ARCH=="X86_64")
Rank         = Mips
GetEnv       = True

Initialdir   = $dir
Input        = /dev/null
Output       = $ldir/$outfile.out
Error        = $ldir/$outfile.err
Log          = /tmp/frankma/$outfile.log
Notify_user  = frankma@mit.edu

+AccountingGroup = "group_cmshi.frankma"
transfer_input_files = macros.tar
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT

Queue
EOF

sleep 0
cat $subfile

echo Executable   = $dir/$1
echo Arguments    = $2 $3 $4 $5 $6 $7 $8 $9


condor_submit $subfile
sleep 0
rm $subfile