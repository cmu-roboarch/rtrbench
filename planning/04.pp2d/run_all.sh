#! /usr/bin/env bash

BINARY="pp2d.out"
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
    local GRID=$2
    local WEIGHT=$3
    local HEURISTIC=$4
    local MAP_SCALE=$5
    local ROB_SCALE=$6

    mkdir -p "$OUTPUT_DIRECTORY"
    local MAP_NAME=$(basename "$INPUT_FILE" .map)

    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}map-${MAP_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}g-${GRID}_"
    OUTPUT_FILE="${OUTPUT_FILE}w-${WEIGHT}_"
    OUTPUT_FILE="${OUTPUT_FILE}h-${HEURISTIC}_"
    OUTPUT_FILE="${OUTPUT_FILE}mscal-${MAP_SCALE}_"
    OUTPUT_FILE="${OUTPUT_FILE}rscal-${ROB_SCALE}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--map=$INPUT_FILE "
    ARGUMENTS="$ARGUMENTS--grid=$GRID "
    ARGUMENTS="$ARGUMENTS--weight=$WEIGHT "
    ARGUMENTS="$ARGUMENTS--heuristic=$HEURISTIC "
    ARGUMENTS="$ARGUMENTS--scale-map=$MAP_SCALE "
    ARGUMENTS="$ARGUMENTS--scale-robot=$ROB_SCALE "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

ALL_GRIDS=(4 8)
ALL_WEIGHTS=(1 10)
ALL_HEURISTICS=("Euclidean" "Manhattan")
ALL_MAP_SCALES=(1 2)
ALL_ROB_SCALES=(1 2)

for INPUT_FILE in $INPUT_DIRECTORY/*; do
    for GRID in "${ALL_GRIDS[@]}"; do
        for WEIGHT in "${ALL_WEIGHTS[@]}"; do
            for HEURISTIC in "${ALL_HEURISTICS[@]}"; do
                for MAP_SCALE in "${ALL_MAP_SCALES[@]}"; do
                    for ROB_SCALE in "${ALL_ROB_SCALES[@]}"; do
                        execute "$INPUT_FILE" "$GRID" "$WEIGHT" "$HEURISTIC" "$MAP_SCALE" "$ROB_SCALE" &
                    done
                done
            done
        done
    done
done

wait

echo "**************************************"

exit 0
