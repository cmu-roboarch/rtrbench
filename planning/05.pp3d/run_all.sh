#! /usr/bin/env bash

BINARY="pp3d.out"
INPUT_DIRECTORY="./input-maps"
OUTPUT_DIRECTORY="./output-paths"

if [[ ! -f $BINARY || ! -d $INPUT_DIRECTORY ]]; then
    echo "Missing required files!"
    echo "Binary: $BINARY ?"
    echo "Input directory: $INPUT_DIRECTORY ?"
    exit 1
fi

execute() {
    local INPUT_FILE=$1
    local WEIGHT=$2
    local HEURISTIC=$3

    mkdir -p "$OUTPUT_DIRECTORY"
    local MAP_NAME=$(basename "$INPUT_FILE" .map)

    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}map-${MAP_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}w-${WEIGHT}_"
    OUTPUT_FILE="${OUTPUT_FILE}h-${HEURISTIC}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--map=$INPUT_FILE "
    ARGUMENTS="$ARGUMENTS--weight=$WEIGHT "
    ARGUMENTS="$ARGUMENTS--heuristic=$HEURISTIC "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

ALL_WEIGHTS=(1 10)
ALL_HEURISTICS=("Euclidean" "Manhattan")

for INPUT_FILE in $INPUT_DIRECTORY/*; do
    for WEIGHT in "${ALL_WEIGHTS[@]}"; do
        for HEURISTIC in "${ALL_HEURISTICS[@]}"; do
            execute "$INPUT_FILE" "$WEIGHT" "$HEURISTIC" &
        done
    done
done

wait

echo "**************************************"

exit 0
