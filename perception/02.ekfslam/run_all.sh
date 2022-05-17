#! /usr/bin/env bash

BINARY="ekfslam.out"
INPUT_DIRECTORY="./input-logs"
OUTPUT_DIRECTORY="./output-logs"

if [[ ! -f $BINARY ]]; then
    echo "Missing required files!"
    echo "Binary: $BINARY ?"
    exit 1
fi

execute() {
    local IN_LOG=$1

    local LOG_NAME=$(basename "$IN_LOG" .txt)

    mkdir -p "$OUTPUT_DIRECTORY"
    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}inlog-${LOG_NAME}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--log=$IN_LOG "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

for INPUT_FILE in $INPUT_DIRECTORY/*; do
    execute "$INPUT_FILE"
done

echo "**************************************"

exit 0
