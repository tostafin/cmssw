filename=7895_ls_fixed

echo  > ${filename}.out
# while read iov; do
for iov in 7115257755601993728 7115343547573731328; do
    echo -n "$iov;" >> ${filename}.out 
    # cmsRun LHCInfoAnalyzer_sqlite.py $iov >> ${filename}.out
    cmsRun ../test/LHCInfoPerLSCsvAnalyzer_cfg.py tag=ls_run3_endFill_fixed_2 \
    source=sqlite_file:ls_run3_fixed_pr.db timestamp=$iov >> ${filename}.out
done
# done < 8019_ls_only_iovs.txt

# grep -E '^(LHC fill|processed iov timestamp|Run number|Crossing angle|Beta star|Delivered Luminosity|Lumi section)' ${filename}.out  > ${filename}_grep.out 
# grep -E 'processed iov timestamp|LHC fill|Bunches in|Bunches in|Colliding bunches|Target bunches|Fill type|Particle type|Particle type|Crossing angle|Beta star|Average Intensity|Average Intensity|Energy \(GeV|Delivered Luminosity|Recorded Luminosity|Instantaneous Luminosity|Instantaneous Luminosity|Creation time|Begin time|End time|Injection scheme|LHC State|LHC Comments|CTPPS Status|Lumi section|Luminosity per|Beam 1|Beam 2|Beam 1|Beam 2|Bunches filled|Bunches filled' ${filename}.out  > ${filename}_grep.out 
# grep -E 'LHCInfo' ${filename}.out  > ${filename}_grep.csv 



# filename=8614_ls_fixed_content_csv_real

# echo  > ${filename}.out
# while read iov; do
#     echo -n "$iov;" >> ${filename}.out 
#     # cmsRun LHCInfoAnalyzer_sqlite.py $iov >> ${filename}.out
#     cmsRun ../test/LHCInfoPerLSCsvAnalyzer_cfg.py tag=8614_ls_endFill_fix2_4 \
#     source=sqlite_file:ls_8614_fix2_2.db timestamp=$iov >> ${filename}.out
# done < 8614_ls_endFill_fix2_only_iovs.txt