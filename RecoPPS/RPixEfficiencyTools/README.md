## Project setup 
Instruction on how to setup and run efficiency suite in CMSSW_11_3_2 environment:
1. Preprare CMSSW_11_3_2 environment with: cmsrel CMSSW_11_3_2
2. Go to newly created directory: cd CMSSW_11_3_2
3. Setup cms environmental variables: cmsenv
4. Merge with the topic containing efficiency suite: git cms-merge-topic varsill:from-CMSSW_11_3_2
5. Compile the solution: scram b -j10
6. Switch directory to the one containing the source code: cd src/RecoPPS/RPixEfficiencyTools
7. Create directories for files used during the run: mkdir InputFiles OutputFiles Jobs LogFiles
8. Prepare the input .dat file for the chosen era with <era name>. For instance, you can specify this file to load all the input .root files from the chosen directory by typing:
	ls /path/to/your/input/files/*.root | sed 's/^/file:/' > InputFiles/Era<era name>.dat
9. *(probably outdated to delete)*bash submitEfficiencyAnalysisEra.sh <era name>


## How to test Efficiency Analyis and Reference Analysis 
1. Set up CMSSW_12_4_0 project
	1. Remember to run cmsenv at src directory
	2. Remember to create needed directories:
		```mkdir InputFiles OutputFiles Jobs LogFiles```
2. Prepare test file and test setup:
	1. **file** *(note/TODO: this file makes sense for EA, but doesn't make sense for RA please find a better file )* `/eos/project-c/ctpps/subsystems/Pixel/RPixTracking/EfficiencyCalculation2018/ReRecoEras_CMSSW_10_6_10/EGamma/re-MINIAOD/ctpps_2018A_pixelefficiency_singleele200630_073503/0000/reMINIAOD_A.root`
	2. **file list to analyse** Just create dat file with paths you want to analyse.
	```TODO```
	3. **Set up config files**
		1. **Global tag** `123X_dataRun2_v4` for now it requires to set a global tag in each python config. 
		2. **Process**  `eras.Run2_2018`
		3. **Producer Tag** `CTPPSTestProtonReconstruction`
		4. **MaxEvents** as test file is big just set maxEvents to smaller value ex. `100000`

3. Run Efficiency analysis (refered as EA).
	1. **Run EA (efficiency analysis) worker**
		```cmsRun python/EfficiencyAnalysisDQMWorker_cfg.py sourceFileList=$1 outputFileName=tmp.root bunchSelection=NoSelection``
	2. **Run EA (efficiency analysis) harvester** To run EA you need to specify 5 paramteres
		1. **inputFileName** .root output file of EA Worker
		2. **outputDirectoryPath** where to save output of root file 
		3. **workflow** ex. efficiency, timing resolution
		4. **dataPeriod** ex. `runNumber` `lumisection`
		5. **campaign** provides a unique naming for distinction between same workflow ex. `25_07_22`
	```cmsRun python/EfficiencyAnalysisDQMHarvester_cfg.py inputFileName=temp.root outputDirectoryPath=OutputFiles workflow=randomNameWorkflow dataPeriod=dumbDataPeriod campaign=fooCampaignName```

4. Run Reference Analysis (refered as RA)
	1. **Run RA worker** 
		1. **sourceFileList** self describing
		2. **outputFileName** self describing 
		3. **efficiencyFileName** .root file output of EA Harvester
	```cmsRun python/ReferenceAnalysisDQMWorker_cfg.py sourceFileList=./InputFiles/Era2.dat outputFileName=tmp_reference_anailsis_worker_output.root efficiencyFileName=./outputFiles/DQM_V0001_R000999999__fooCampaignName__randomNameWorkflow__dumbDataPeriod.root```

	2. **Run RA harvester**

	TODO: Probalby outputDirectoryPath doesn't change the output path. 
	```cmsRun python/ReferenceAnalysisDQMHarvester_cfg.py inputFileName=tmp_reference_anailsis_worker_output.root outputDirectoryPath=OutputFiles```

# TODO
1. [ ]  method `print_information_about_LHC_run` is not extracted and is not working in modules 
	TODO: extract the method to other file and fix it to print correct informations
2. [ ] Global tag is set in every python file separately - if you change a global tag you need to edit four files!!!

