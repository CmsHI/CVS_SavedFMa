#!/bin/bash -
if [ $# -eq 0 ]; then
   echo "Usage:"
   echo "  create_custom_cfg.sh <original.py> [cus1.py cus2.py cus3.py ...]"
   echo "Default:"
   echo "  adds output_cfg_summary.py"
   exit 1
fi

script=create_custom_python_cfg.sh
echo "#!/bin/bash -" > $script
echo "#command line: create_custom_cfg.sh $@" >> $script

cfg0=$1
newcfg=custom_${cfg0##*/}

shift

if [ $# -eq 0 ]; then
   echo "add_custom_cfg.sh $cfg0 $mySeqPython/output_cfg_summary.py" >> $script
else
   echo "add_custom_cfg.sh $cfg0 $1" >> $script
   shift
   while [ $# -gt 0 ]; do
      echo "add_custom_cfg.sh $newcfg $1" >> $script
      shift
   done
   echo "add_custom_cfg.sh $newcfg $mySeqPython/output_cfg_summary.py" >> $script
fi

chmod +x $script

# run script
./$script
