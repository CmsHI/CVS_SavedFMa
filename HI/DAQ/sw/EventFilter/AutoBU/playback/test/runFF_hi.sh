# 2009/7/30
# $@: -playback raw.root, for now
# sm has some errors ..., currently running without

# check sw, currently initialize uses hard-coded sw path
echo ${CMSSW_BASE##*/} | grep pro
if [ $? -ne 0 ]; then
   echo "You are not using pro sw"
   echo "inizialize uses pro"
   return 1
fi

./playback_scripts/initialize_hi -python -noSM "$@"
