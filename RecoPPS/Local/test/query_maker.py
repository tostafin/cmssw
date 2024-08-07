import subprocess
import json
import sys

with open(sys.argv[1], 'r') as f:
    lumi_data = json.load(f)

all_files = ""

for run, _ in lumi_data.items():
    query = f'file dataset=/JetMET0/Run{sys.argv[2]}-PromptReco-v1/MINIAOD run={run}'
    # query = f'file dataset=/JetMET1/Run2024B-PromptReco-v1/AOD run={run}'
    command = f'dasgoclient --query=\'{query}\''
    result = subprocess.run(command,shell=True,capture_output=True, text=True)
    if result.stdout:
        all_files+=result.stdout
    else:
        sys.stderr.write('couldn\'t get .root files for run '+run+'\n')

# Print all .root files
print(str(all_files).replace('\n',',')[:-1])
