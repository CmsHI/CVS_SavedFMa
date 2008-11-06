echo "===Check XDAQ installation:"
env | grep XDAQ
echo "===Check HOST:"
if [ $HOST ]; then
   echo $HOST
else
   echo "Need to define host:"
   echo "export HOST=$(hostname)"
fi

