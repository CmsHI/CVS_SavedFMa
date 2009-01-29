if [ $1 ]; then
   host=$1
else
   host='fuval-C2F12-17'
fi
firefox $host:40000&

