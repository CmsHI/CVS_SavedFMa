echo "===Check XDAQ installation:==="
env | grep XDAQ

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

