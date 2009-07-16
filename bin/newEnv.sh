#!/bin/bash -
if [ $# -eq 0 ]; then
   echo usage:
   echo '  source newEnv.sh <cmssw_project_area>'
   return 1
fi

cat > env.sh << EOF
setcms
cd $1
eval \`scramv1 ru -sh\`
cd -
EOF

