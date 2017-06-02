export SCRAM_ARCH=slc6_amd64_gcc530
export BUILD_ARCH=slc5_amd64_gcc462
export VO_CMS_SW_DIR=/afs/cern.ch/cms
source /afs/cern.ch/cms/cmsset_default.sh
cd /afs/cern.ch/work/n/nminafra/CMSSW/DQM/CMSSW_9_2_0_patch2/src
eval `scramv1 runtime -sh`
cd -
cmsRun /afs/cern.ch/user/n/nminafra/Work/CMSSW/DQM/CMSSW_9_2_0_patch2/src/DQMRun2_295626/dqm_run.py
mv DQM_V*.root /afs/cern.ch/user/n/nminafra/Work/public/DQMOut/Selected2/