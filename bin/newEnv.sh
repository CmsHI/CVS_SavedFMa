#!/bin/bash -
cat > env.sh << EOF
setcms
cd $1
eval \`scramv1 ru -sh\`
cd -
EOF

