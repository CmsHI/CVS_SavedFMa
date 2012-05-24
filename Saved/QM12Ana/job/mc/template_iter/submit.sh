#!/bin/sh

dataset=/mnt/hadoop/cms/store/user/yetkin/MC_Production/Pythia120_HydjetDrum_mix01/RECO/

counter=1

destination=/mnt/hadoop/cms/store/user/frankmalocal/forest/Hydjet18/DiJet120_iterTrk_v1

mkdir -p $destination

dir=$dataset
for subdir in / # `ls $dir`
  do
  for infile in `ls $dir/$subdir | grep root`
    do
      if [ $counter -ge $1 ] && [ $counter -le $2 ]; then
	  echo "Submitting process input file: $infile"
	  echo $counter >> Submitted
	  lfn=$dir/$subdir/$infile
          randomNumber=$counter
          outfile=$infile

	  Error=`echo $outfile | sed "s/root/err/g"`
	  Output=`echo $outfile | sed "s/root/out/g"`
	  Log=`echo $outfile | sed "s/root/log/g"`        

	  
#          if [ -f $destination/$outfile ]; then
	  if [ -f $destination/$outfile ]; then
	      echo "Output already exists : $outfile"
	  else

	      echo "Input is : $infile"
	      echo "Output is : $outfile"
	      echo "LFN is : $lfn"
	      echo "----------------------"
	      
   
	      
#+IsMadgraph = 1  
	      
# Condor submit file
	      cat > subfile <<EOF

Universe       = vanilla

# files will be copied back to this dir
Initialdir     = .

# run my script
Executable     = run.sh

+AccountingGroup = "group_cmshi.frankma"
#+IsMadgraph = 1

Arguments      = $lfn $outfile $destination $randomNumber \$(Process)
# input files. in this case, there are none.
Input          = /dev/null

# log files
Error          = $Error
Output         = $Output
Log            = $Log

# get the environment (path, etc.)
Getenv         = True

# prefer to run on fast computers
Rank           = kflops

Requirements = (Machine=="t2bat0147.cmsaf.mit.edu") || (Machine=="t2bat0148.cmsaf.mit.edu") || (Machine=="t2bat0150.cmsaf.mit.edu") || (Machine=="t2bat0151.cmsaf.mit.edu") ||-(Machine=="t2bat0152.cmsaf.mit.edu") || (Machine=="t2bat0153.cmsaf.mit.edu") || (Machine=="t2bat0154.cmsaf.mit.edu") ||-(Machine=="t2bat0155.cmsaf.mit.edu") || (Machine=="t2bat0156.cmsaf.mit.edu") ||-(Machine=="t2bat0157.cmsaf.mit.edu") ||-(Machine=="t2bat0158.cmsaf.mit.edu") || (Machine=="t2bat0160.cmsaf.mit.edu") ||-(Machine=="t2bat0161.cmsaf.mit.edu") || (Machine=="t2bat0162.cmsaf.mit.edu") || (Machine=="t2bat0163.cmsaf.mit.edu") ||-(Machine=="t2bat0164.cmsaf.mit.edu") || (Machine=="t2bat0165.cmsaf.mit.edu") || (Machine=="t2bat0166.cmsaf.mit.edu") ||-(Machine=="t2bat0167.cmsaf.mit.edu") || (Machine=="t2bat0168.cmsaf.mit.edu") || (Machine=="t2bat0170.cmsaf.mit.edu") ||-(Machine=="t2bat0171.cmsaf.mit.edu") || (Machine=="t2bat0172.cmsaf.mit.edu") ||-(Machine=="t2bat0173.cmsaf.mit.edu") ||-(Machine=="t2bat0174.cmsaf.mit.edu") || (Machine=="t2bat0175.cmsaf.mit.edu") ||-(Machine=="t2bat0176.cmsaf.mit.edu") || (Machine=="t2bat0177.cmsaf.mit.edu") || (Machine=="t2bat0178.cmsaf.mit.edu")



# should write all output & logs to a local directory
# and then transfer it back to Initialdir on completion
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT
# specify any extra input files (for example, an orcarc file)
transfer_input_files    = cfg.py

Queue
EOF

# submit the job
condor_submit subfile
fi
fi
counter=$(($counter + 1))
done
done

