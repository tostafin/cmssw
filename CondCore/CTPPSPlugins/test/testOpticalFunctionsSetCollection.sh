#!/bin/bash

if [ "$1" == "run" ]
then
    mkdir -p CondCore/CTPPSPlugins/test/results
    if [ -f *.png ]; then    
    rm *.png
    fi

    echo "Testing LHCOptics txt"

    python3 CondCore/Utilities/scripts/getPayloadData.py \
        --plugin pluginLHCOpticalFunctionsSetCollection_PayloadInspector \
        --plot plot_OpticalFunctionsInfoPrint_RP123 \
        --tag PPSOpticalFunctions_offline_v7 \
        --time_type Run \
        --iovs '{"start_iov": "273725", "end_iov": "352322"}' \
        --db Prod \
        --test
        mv *.png CondCore/CTPPSPlugins/test/results/LHCOpticalFunctionsCollection_Payload_TextInfo.png  
    
    
    echo "Testing LHCOptics history plot"
    
    for func in 'evx' 'eLx' 'e14' 'exd' 'evpx' 'eLpx' 'e24' 'expd' 'e32' 'evy' 'eLy' 'eyd' 'e42' 'evpy' 'eLpy' 'eypd'
    do
        python3 CondCore/Utilities/scripts/getPayloadData.py \
            --plugin pluginLHCOpticalFunctionsSetCollection_PayloadInspector \
            --plot plot_OpticalFunction_History_A150RP23_${func} \
            --tag PPSOpticalFunctions_offline_v7 \
            --time_type Run \
            --iovs '{"start_iov": "273725", "end_iov": "352322"}' \
            --db Prod \
            --test 2> CondCore/CTPPSPlugins/test/results/optics_history_example_${func}.json   
    done        

    python3 CondCore/CTPPSPlugins/test/graph_check.py
    
    python3 CondCore/Utilities/scripts/getPayloadData.py \
        --plugin pluginLHCOpticalFunctionsSetCollection_PayloadInspector \
        --plot plot_OpticalFunctionsRPs\
        --tag PPSOpticalFunctions_offline_v7 \
        --time_type Run \
        --iovs '{"start_iov": "273725", "end_iov": "352322"}' \
        --db Prod \
        --test  
        mv *.png CondCore/CTPPSPlugins/test/results/LHCOpticalFunctionsCollection_RPs_History.png  

elif [ "$1" == "clear" ]
then
    rm -rf CondCore/CTPPSPlugins/test/results

else 
    echo "Wrong option"
fi
