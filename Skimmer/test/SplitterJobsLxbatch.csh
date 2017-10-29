#!/bin/csh
set RUN = $1
set PD = $2

set PWD = `pwd`
set max = `wc -l < Run${RUN}_${PD}_cff.py`

echo $max
set i = 1

mkdir split_jobs_${RUN}_${PD}

while ( $i <= ${max})

set readFiles = `cat Run${RUN}_${PD}_cff.py | sed -n ${i}p`
cat RunMonitor.py > RunMonitor_${RUN}_${PD}_${i}.py
sed -e "s/vstring(readFiles)/vstring(${readFiles})/" RunMonitor_${RUN}_${PD}_${i}.py
set cfg = RunMonitor_${RUN}_${PD}_${i}.py

bsub -q8nh<<EOF

#!/bin/csh
setenv X509_USER_PROXY "${PWD}/x509_proxy" 
voms-proxy-init --voms cms --noregen
cd ${PWD}/split_jobs_${RUN}_${PD}
mv ../${cfg} .
cmsenv
cd -
cmsRun ${PWD}/split_jobs_${RUN}_${PD}/$cfg
mv ntuple_RunOutputName.root ${PWD}/split_jobs_${RUN}_${PD}/ntuple_RunOutputName_${i}.root
mv outputfile_RunOutputName.root ${PWD}/split_jobs_${RUN}_${PD}/outputfile_RunOutputName_${i}.root
rm ${PWD}/split_jobs_${RUN}_${PD}/$cfg

EOF
        @ i++
end
