## Project setup 
Instruction on how to setup and run efficiency suite in CMSSW_12_4_0 environment:

1. Preprare CMSSW_12_4_0 environment with: `cmsrel CMSSW_12_4_0`
2. Go to newly created directory: `cd CMSSW_12_4_0`
3. Setup cms environmental variables(repeat every time you log in): `cmsenv`
4. Merge with the main branch containing efficiency suite: `git cms-merge-checkout CTPPS:MO/timing/migration_to_CMSSW_12_4_6_clean_branch` #(TODO change it to the main )
5. Compile the solution `scram b -j10`
6. Switch directory to the one containing the source code: `cd src/Analyzer/DiamondTimingAnalyzer`

## Run rolling calibrations: 
### Applying time shift 
In current design you need to apply a time shift to data. 
1. Set files you want to analyze in `apply_time_shift.py`
2. Make sure you are using the newest tag for TimingCalibration (current is   `PPSDiamondTimingCalibration_Run3_recovered_v1`)
3. run `apply_time_shift.py`
```
cmsRun apply_time_shift.py
```
4. Use AOD.root as an input file for rolling calibration. 
*(note: you can turn off DQM plots production for faster data processing)*
### Rolling calibration


## Manual tests
1. Running worker  

```bash
cmsRun python/worker.py
```

2. Running harvester
```bash 
cmsRun python/harvester.py rootFiles=file:run_output.root 
```

3. Submit to CRAB:
```cmsemv```
```voms-proxy-init -voms cms -rfc  -valid 192:00```
```crab submit submitTimingToCRAB.py ```


## Project Structure
### Flow
- **TimeShift:** *(Just temporary step)*
    - config file is in `python/apply_time_shift.py`
    - It applies time shift to root data. The end goal is that the step should be applied already to dataset, but for now we have a workaround.

- **Worker:**
    - config file is in `python/worker.py`
        - Input:
        rootInput (time_shift should be applied) - it's the same for all iterations, 
        planesConfig - a [json file](TODO_ADD_URL) with the structure of detectors,
        useDB, calibInput, sqlFileName - specify one of these parameters - each can provide time shift (BE CAREFUL: not tested with SQL file)
        validOOT - TODO: probably to delete -(BE CAREFUL it's not set in the code)
        outputFileName
        maxEventsToProcess - reduce number of events to process, useful for testing purposes 
    - worker .cc file:
        - TOOD: descibire what it does
        - consumes 6 tags (check `DiamondTimingWorker.cc for details)
        - depends on DiamondTimingCalibration(TOOD: descibire what it does) and DiamondDetectorClass(TODO: desciribe what it does)

- **Harvester:**
    - config file is in `python/harvester.py`
        - Input:
    - harvester .cc file:

        

### Config.json
Config.json describes the structure of detectors. 
- Sector - left of right arm
- station - the pot in which the detector are located
- plane - the detector contains planes - 
- channel - each plane is divided into channels. They are counted in U-shape, due to the phisical connection structure. 

## More Details:
### Finding Test File:
### Useful tips: 
- add debug flag `<flags CXXFLAGS="-O0 -g -DDEBUG"/>` in `BuildFile.xml` to produce more logs
- in config files you can add for more logs
```
process.MessageLogger.cerr.threshold = "INFO"
process.MessageLogger.cerr.INFO.limit = -1
```
- logs to file - append: 
```
 2>&1 | tee Logs/worker_log_5.txt
```
