#!/bin/csh
set DIR = `pwd`

setenv X509_USER_PROXY "${DIR}/x509_proxy"
voms-proxy-init --voms cms --noregen

cd ${DIR}
eval `scramv1 ru -csh`
cd -
env

cmsRun ${DIR}/RunMonitor.py
cp -r Run*/ ${DIR}/.
cp -r monitor*root ${DIR}/.
cp *txt ${DIR}/.
