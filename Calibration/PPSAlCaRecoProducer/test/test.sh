# I've also tried simpler tests without L1Reco stage, namely:
# `cmsDriver.py step2  -s ALCAPRODUCER:PPSCalMaxTracksExpress,ENDJOB \``
cmsDriver.py step2  -s L1Reco,ALCAPRODUCER:PPSCalMaxTracksExpress,ENDJOB \
--process ALCARECO \
--scenario pp \
--era ctpps_2018 \
--conditions auto:run3_data_express \
--data  \
--datatier ALCARECO \
--eventcontent ALCARECO \
-n 100  --filein file:/eos/project-c/ctpps/subsystems/Software/Off-line/AlCaTest/outputALCAPPS_single.root \
--fileout file:step2.root