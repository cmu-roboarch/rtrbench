#! /usr/bin/env bash

BINARY="dmp.out"
INPUT_DIRECTORY="./input-trajs"
OUTPUT_DIRECTORY="./output-trajs"

if [[ ! -f $BINARY || ! -d $INPUT_DIRECTORY ]]; then
    echo "Missing required files!"
    echo "Binary: $BINARY ?"
    echo "Input directory: $INPUT_DIRECTORY ?"
    exit 1
fi

execute() {
    local TRAJ_FILE=$1
    local DT=$2

    local TRAJ_NAME=$(basename "$TRAJ_FILE" .traj)
    local WEIGHTS_FILE=$INPUT_DIRECTORY/$TRAJ_NAME.weights
    if [[ ! -f $WEIGHTS_FILE ]]; then
        echo "Couldn't find weights file ${WEIGHTS_FILE}"
        exit 1
    fi

    mkdir -p "$OUTPUT_DIRECTORY"
    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}traj-${TRAJ_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}dt-${DT}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--trajectory=$TRAJ_FILE "
    ARGUMENTS="$ARGUMENTS--weights=$WEIGHTS_FILE "
    ARGUMENTS="$ARGUMENTS--dt=$DT "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

ALL_DTS=(0.001 0.1)

for TRAJ_FILE in $INPUT_DIRECTORY/*.traj; do
    for DT in "${ALL_DTS[@]}"; do
        execute "$TRAJ_FILE" "$DT" &
    done
done

wait

echo "**************************************"

exit 0
