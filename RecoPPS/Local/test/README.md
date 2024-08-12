To run the comparison:
- enter the directory from linux terminal
- fill the **goodLsList.json** with run numbers and LS ranges that will be included in the comparison, like this:
```
{"379391": [[1,5],[200,215]], 
 "379390": [[2,3], [4,6]]
}
```
- run `chmod +x compare_reco_HLT_GT_vs_Prompt_GT.sh`
- run `voms-proxy-init -voms cms -rfc` to create a VOMS proxy
- run `./compare_reco_HLT_GT_vs_Prompt_GT.sh goodLsList.json 2024B` (replace 2024B with relevant era if needed)
- compare **lhcinfo_\*.root** and **reco_protons_hist_\*.root**
- output of rereco processes will be stored in  **output_\*.txt** files
- to change GTs used in comparison modify `globalTag=xyz` field in lines 11 and 12 of `compare_reco_HLT_GT_vs_Prompt_GT.sh`
- to change the das query modify line 11 of query_maker.py