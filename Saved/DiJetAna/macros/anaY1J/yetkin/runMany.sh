

for run in `seq $1 $2`
do

cmsRun cfg.py randomNumber=$run output=treefile$run.root
done
