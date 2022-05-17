#! /usr/bin/env bash

BINARY="movtar.out"
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

    mkdir -p "$OUTPUT_DIRECTORY"
    local MAP_NAME=$(basename "$INPUT_FILE" .txt)

    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}map-${MAP_NAME}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--map=$INPUT_FILE "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

for INPUT_FILE in $INPUT_DIRECTORY/*; do
    execute "$INPUT_FILE" &
done

wait

echo "**************************************"

exit 0
