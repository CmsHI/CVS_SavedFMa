ct=0; for i in `cat JetFragAna.C | grep var.*= | awk -F= {print }`; do echo "var[$ct]=$i"; ct=$((ct+1)); done
