
export SCRAM_ARCH=slc5_amd64_gcc434
source /osg/app/cmssoft/cms/cmsset_default.sh

cd /net/hidsk0001/d00/scratch/frankma/work/jet/itertrkForest_442p3/src

eval `scramv1 runtime -sh`
cd -

infile=$1
outfile=$2
destination=$3
randomN=$4

echo "Processing..."
echo "Random number : $randomNumber"
echo "cmsRun cfg.py files=$infile output=$outfile"
allnight=$(($RANDOM / 1000))
echo sleeping... $allnight

cmsRun cfg.py inputFiles=file:$infile outputFile=$outfile randomNumber=$randomN

mv $outfile $destination/

echo "Done!"
echo "Copying output files to " $destination

