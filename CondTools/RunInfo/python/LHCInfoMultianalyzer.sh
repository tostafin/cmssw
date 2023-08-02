filename=9019_fixed_content
extention=csv

# echo  > ${filename}.${extention}
while read iov; do
    echo -n "$iov;" >> ${filename}.${extention} 
    # cmsRun LHCInfoAnalyzer_sqlite.py $iov >> ${filename}.out
    cmsRun LHCInfoAnalyzer.py tag=LHCInfoEndFill_Run3_validation_v2 \
    source=sqlite_file:lhcinfo_9019_fixed.db timestamp=$iov >> ${filename}.${extention}
done < rest_9019_only_iovs_fixed.txt

# grep -E '^(LHC fill|processed iov timestamp|Run number|Crossing angle|Beta star|Delivered Luminosity|Lumi section)' ${filename}.out  > ${filename}_grep.out 
# grep -E 'processed iov timestamp|LHC fill|Bunches in|Bunches in|Colliding bunches|Target bunches|Fill type|Particle type|Particle type|Crossing angle|Beta star|Average Intensity|Average Intensity|Energy \(GeV|Delivered Luminosity|Recorded Luminosity|Instantaneous Luminosity|Instantaneous Luminosity|Creation time|Begin time|End time|Injection scheme|LHC State|LHC Comments|CTPPS Status|Lumi section|Luminosity per|Beam 1|Beam 2|Beam 1|Beam 2|Bunches filled|Bunches filled' ${filename}.out  > ${filename}_grep.out 
# grep -E 'LHCInfo' ${filename}.out  > ${filename}_grep.csv 

#archive
#     cmsRun LHCInfoAnalyzer.py tag=LHCInfoEndFill_Run3_validation_v1 \
#     source=sqlite_file:lhcinfo_pop_test_8614.db timestamp=$iov >> ${filename}.out

    # cmsRun LHCInfoAnalyzer.py tag=LHCInfoEndFill_Run3_validation_v2 \
    # source=sqlite_file:lhcinfo_8614_fixed.db timestamp=$iov >> ${filename}.${extention}