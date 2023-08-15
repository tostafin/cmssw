LOG_FILE=$1
echo "Number of invalid payloads found in the logs:"
for CONDITION in "crossingAngle == 0 for both X and Y: " "crossingAngle != 0 for both X and Y: " "negative crossingAngle: " "negative betaStar: " ; do
    echo -n "$CONDITION max in one fill: "
    (cat $LOG_FILE | grep -E "$CONDITION" | awk '{print $NF}' ; echo 0) | sort -gr | head -n 1
    echo -n "$CONDITION total: "
    (cat $LOG_FILE | grep -E "$CONDITION" | awk '{print $NF}'; echo 0) | paste -sd+ | bc
done

