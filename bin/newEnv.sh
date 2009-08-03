#!/bin/bash -
if [ $# -eq 0 ]; then
   echo usage:
   echo '  source newEnv.sh <cmssw_project_area>'
   exit 1
fi

cat > env.sh << EOF
rundir=\$PWD
cd $1
eval \`scramv1 ru -sh\`
. \$SavedFMa/bin/env/mycmssw_env.sh
cd \$rundir
EOF

