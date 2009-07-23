#!/bin/bash -

#
# - Env needs:
#   * CMSSW
#
print_help () {
   echo usage:
   echo "  getOfflHLT.sh [-c] [-r release] <-f file:raw.root> [-v]"
}

if [ $# -eq 0 ]; then
   print_help
   exit 1
fi

# initialze variables
checkConfDB="false"  release=  dbCfg=  
infile=  verbose=0

# set variables
while [ $# -gt 0 ]; do
   case $1 in 
      -c)  checkConfDB="true"
      ;;
      -r)  release=$2
      shift
      ;;
      -f)  infile=$2
      shift
      ;;
      -v)  verbose=1
      ;;
      --)  shift # By convention, --ends options
      break
      ;;
      -*)  echo $0: $1: unrecognized option >&2
      ;;
      *)  break # nonoption arguemtn, break while loop
      ;;
   esac
   shift # setup for next iteration
done


# === set cfg ===
# - reference:
#   * https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideGlobalHLT
case $release in
   311)  #dbCfg=/dev/CMSSW_3_2_0/pre1/HIon_V2/V2 #official
         dbCfg=/user/frankma/HI/cmssw311/devHIon/V3 #private
   ;;
   321)  dbCfg=/dev/CMSSW_3_2_0/pre1/HIon_V20
   ;;
   *)      echo release $release: no menu set
   exit 1
   ;;
esac

if [ $verbose == 1 ]; then echo checkConfDB $checkConfDB; fi
# do we want to just check confDB?
if [ $checkConfDB == "true" ]; then
   echo === $dbCfg versions: ===
   edmConfigFromDB --configName $dbCfg --versions
   exit 0
fi

# === Start getting the menu ===
# check input file
if [ -z $infile ]; then
   echo "no infile"
   print_help
   exit 1
fi

# set output name
name=r$release
if echo $dbCfg | grep -q '/V[[:alnum:]]$'; then
   name=${name}_`echo $dbCfg | awk -F/ '{print $(NF-1) $(NF)}'`
else
   name=${name}_${dbCfg##/*/}
fi
name=${name}_cfg.py

# Now all variables are finished being set do we want
# to display them?
if [ $verbose -ge 1 ]; then
   echo release $release
   echo dbCfg $dbCfg
   echo infile: $infile
   echo output: $name
fi


# finally, run
edmConfigFromDB --configName $dbCfg --input $1 > $name
