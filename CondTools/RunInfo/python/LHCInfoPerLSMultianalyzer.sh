filename=8614_ls_content_csv

echo  > ${filename}.out
while read iov; do
    echo -n "$iov;" >> ${filename}.out 
    # cmsRun LHCInfoAnalyzer_sqlite.py $iov >> ${filename}.out
    cmsRun ../test/LHCInfoPerLSCsvAnalyzer_cfg.py tag=LHCInfoPerLS_PopCon_test \
    source=sqlite_file:ls_8614_fixed.db timestamp=$iov >> ${filename}.out
done < 8614_ls_only_iovs.txt

# grep -E '^(LHC fill|processed iov timestamp|Run number|Crossing angle|Beta star|Delivered Luminosity|Lumi section)' ${filename}.out  > ${filename}_grep.out 
# grep -E 'processed iov timestamp|LHC fill|Bunches in|Bunches in|Colliding bunches|Target bunches|Fill type|Particle type|Particle type|Crossing angle|Beta star|Average Intensity|Average Intensity|Energy \(GeV|Delivered Luminosity|Recorded Luminosity|Instantaneous Luminosity|Instantaneous Luminosity|Creation time|Begin time|End time|Injection scheme|LHC State|LHC Comments|CTPPS Status|Lumi section|Luminosity per|Beam 1|Beam 2|Beam 1|Beam 2|Bunches filled|Bunches filled' ${filename}.out  > ${filename}_grep.out 
# grep -E 'LHCInfo' ${filename}.out  > ${filename}_grep.csv 