#! /usr/bin/env bash

BINARY="srec.out"
OUTPUT_DIRECTORY="./output-trajs"

if [[ ! -f $BINARY ]]; then
    echo "Missing required files!"
    echo "Binary: $BINARY ?"
    exit 1
fi

execute() {
    local SCENE_PATH=$1
    local CAMERA_FILE=$2
    local DOWNSAMPLE=$3

    local SCENE_NAME=${SCENE_PATH%%/}
    local CAMERA_NAME=${CAMERA_FILE%%_*}

    mkdir -p "$OUTPUT_DIRECTORY"
    local OUTPUT_FILE="$OUTPUT_DIRECTORY/"
    OUTPUT_FILE="${OUTPUT_FILE}scene-${SCENE_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}camera-${CAMERA_NAME}_"
    OUTPUT_FILE="${OUTPUT_FILE}dsamp-${DOWNSAMPLE}"
    OUTPUT_FILE="${OUTPUT_FILE}.txt"

    local ARGUMENTS=""
    ARGUMENTS="$ARGUMENTS--path=$SCENE_PATH "
    ARGUMENTS="$ARGUMENTS--camera=$CAMERA_FILE "
    ARGUMENTS="$ARGUMENTS--downsample=$DOWNSAMPLE "
    ARGUMENTS="$ARGUMENTS--output=$OUTPUT_FILE "

    ./$BINARY $ARGUMENTS
    if [[ $? != 0 ]]; then
        echo "Error in running ./${BINARY} ${ARGUMENTS}"
    fi

    echo "--------------------------------------"
}

ALL_SCENE_DIRS=("living-room")
ALL_CAMERA_FILES=("camera1.json")
ALL_DOWNSAMPLES=(1 2 10)

for SCENE in "${ALL_SCENE_DIRS[@]}"; do
    for CAMERA in "${ALL_CAMERA_FILES[@]}"; do
        for DOWNSAMPLE in "${ALL_DOWNSAMPLES[@]}"; do
            execute "$SCENE" "$CAMERA" "$DOWNSAMPLE"
        done
    done
done

echo "**************************************"

exit 0
