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
        --tag PPSOpticalFunctions_hlt_v9 \
        --time_type Run \
        --iovs '{"start_iov": "273725", "end_iov": "340000"}' \
        --db Prod \
        --test
        #mv *.png CondCore/CTPPSPlugins/test/results/LHCOpticalFunctionsCollection_Payload_TextInfo.png  
    
elif [ "$1" == "clear" ]
then
    rm -rf CondCore/CTPPSPlugins/test/results

else 
    echo "Wrong option"
fi
