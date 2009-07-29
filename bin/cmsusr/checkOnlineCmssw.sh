. /nfshome0/frankma/UserCode/SavedFMa/bin/cmsusr/setfuvalsw.sh
echo "=== Online CMSSW installations ==="
scramv1 list | grep CMSSW
echo
echo "=== onboard rpms ==="
rpm -qa | grep cmssw

if [ $# -ge 1 ]; then
   echo
   echo "=== $1: ==="
   ll /opt/cmssw/$1
   echo
   echo "=== $1/patch/src: ==="
   l /opt/cmssw/$1/patch/src
fi

