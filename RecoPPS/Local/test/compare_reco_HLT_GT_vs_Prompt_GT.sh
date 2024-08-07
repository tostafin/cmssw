if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <arg1> <arg2> where arg1 is a json file teling which LSes from which runs should be considered and arg2 describes which era are the data from"
  exit 1
fi

cmsenv
inputFiles=$(python3 query_maker.py $1 $2)

echo $inputFiles

cmsRun reco_for_comparison.py globalTag='140X_dataRun3_HLT_v3' inputFiles=${inputFiles} rootFilesSuffix='hlt' > output_HLT.txt 2>&1 &
cmsRun reco_for_comparison.py globalTag='140X_dataRun3_Prompt_v4' inputFiles=${inputFiles} rootFilesSuffix='prompt' > output_prompt.txt 2>&1 &

wait
