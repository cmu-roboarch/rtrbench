#! /usr/bin/env bash

BINARY="sym_planner.out"
INPUT_DIRECTORY="./input-problems"
OUTPUT_DIRECTORY="./output-plans"

if [[ ! -f $BINARY || ! -d $INPUT_DIRECTORY ]]; then
    echo "Missing required files!"
    echo "Binary: $BINARY ?"
    echo "Input directory: $INPUT_DIRECTORY ?"
    exit 1
fi

execute() {
    local INPUT_FILE=$1
    local WEIGHT=$2

    mkdir -p "$OUTPUT_DIRECTORY"
    local PROBLEM_NAME=$(basename "$INPUT_FILE" .txt)

    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}problem-${PROBLEM_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}w-${WEIGHT}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--input=$INPUT_FILE "
    ARGUMENTS="$ARGUMENTS--weight=$WEIGHT "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

ALL_WEIGHTS=(0 1 10)

for INPUT_FILE in $INPUT_DIRECTORY/*; do
    for WEIGHT in "${ALL_WEIGHTS[@]}"; do
        execute "$INPUT_FILE" "$WEIGHT" &
    done
done

wait

echo "**************************************"

exit 0
