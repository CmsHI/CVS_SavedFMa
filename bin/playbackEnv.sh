result=0
echo "===Check XDAQ installation:==="
env | grep XDAQ
if [ $? -ne 0 ]; then
   result=2
   echo XDAQ env not found
   echo try to setup scramv1 env.
fi

echo
echo "===Check HOST:"
if [ $HOST ]; then
    echo "Ok. HOST defined:"
    echo $HOST
else
    echo "Need to define host:==="
    echo "export HOST=$(hostname)"
fi

echo
echo "===Check Shared Memory:==="
cat /proc/sys/kernel/shmmax

echo
echo "Result:"
if [ $result -eq 0 ]; then
   echo "  Software env all set for playback"
else
   echo "  warning, env not setup for playback"
fi

exit $result

