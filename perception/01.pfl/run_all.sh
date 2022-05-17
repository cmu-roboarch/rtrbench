#! /usr/bin/env bash

BINARY="pfl.out"
INPUT_MAP_DIRECTORY="./input-maps"
INPUT_MEAS_DIRECTORY="./input-measurements"
OUTPUT_DIRECTORY="./output-logs"

if [[ ! -f $BINARY || ! -d $INPUT_MAP_DIRECTORY || ! -d $INPUT_MEAS_DIRECTORY ]]; then
    echo "Missing required files!"
    echo "Binary: $BINARY ?"
    echo "Input maps: $INPUT_MAP_DIRECTORY ?"
    echo "Input measurements: $INPUT_MEAS_DIRECTORY ?"
    exit 1
fi

execute() {
    local MAP_FILE=$1
    local MEAS_FILE=$2
    local PARTICLES=$3
    local SUBSAMPLE=$4
    local NUM_UPDATES=$5

    local MAP_NAME=$(basename "$INPUT_MAP_DIRECTORY" .dat)
    local MEAS_NAME=$(basename "$INPUT_MEAS_DIRECTORY" .txt)

    mkdir -p "$OUTPUT_DIRECTORY"
    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}map-${MAP_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}meas-${MEAS_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}p-${PARTICLES}_"
    OUTPUT_FILE="${OUTPUT_FILE}s-${SUBSAMPLE}_"
    OUTPUT_FILE="${OUTPUT_FILE}u-${NUM_UPDATES}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--map=$MAP_FILE "
    ARGUMENTS="$ARGUMENTS--measurements=$MEAS_FILE "
    ARGUMENTS="$ARGUMENTS--particles=$PARTICLES "
    ARGUMENTS="$ARGUMENTS--subsample=$SUBSAMPLE "
    ARGUMENTS="$ARGUMENTS--updates=$NUM_UPDATES "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

ALL_PARTICLES=(100 500)
ALL_SUBSAMPLE=(1 5)
ALL_NUM_UPDATES=(100 500)

for MAP_FILE in $INPUT_MAP_DIRECTORY/*; do
    for MEAS_FILE in $INPUT_MEAS_DIRECTORY/*; do
        for PARTICLES in "${ALL_PARTICLES[@]}"; do
            for SUBSAMPLE in "${ALL_SUBSAMPLE[@]}"; do
                for NUM_UPDATES in "${ALL_NUM_UPDATES[@]}"; do
                    execute "$MAP_FILE" "$MEAS_FILE" "$PARTICLES" "$SUBSAMPLE" "$NUM_UPDATES" &
                done
            done
        done
    done
done

wait

echo "**************************************"

exit 0
