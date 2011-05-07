#alias mycmsenv='eval `scramv1 runtime -sh`'
export PYTHONPATH=$PYTHONPATH:$SavedFMa/cmssw
export PYTHONPATH=$PYTHONPATH:$SavedFMa/data

nslookup `hostname` | grep Name | grep '\.cms$'
if [ $? -ne 0 ]; then
   cd $data/DBS/Clients/Python
   . setup.sh
   cd -
fi
