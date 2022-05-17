#! /usr/bin/env bash

FLAGS="-j8"
# FLAGS="performance -j8"
# FLAGS="debug -j8"

if [[ $# -eq 0 ]]; then
    ARG=""
elif [[ $# -eq 1 ]]; then
    ARG=$1
else
    echo "Usage: $0 [benchmark]"
    exit 1
fi

INIT_PATH=$( pwd )

ALL_BENCHMARKS=(
"01.pfl"
"02.ekfslam"
"03.srec"
"04.pp2d"
"05.pp3d"
"06.movtar"
"07.prm"
"08.rrt"
"09.rrtstar"
"10.rrtpp"
"11.sym-blkw"
"12.sym-fext"
"13.dmp"
"14.mpc"
"15.cem"
"16.bo"
)

ALL_PATHS=(
"perception/01.pfl"
"perception/02.ekfslam"
"perception/03.srec"
"planning/04.pp2d"
"planning/05.pp3d"
"planning/06.movtar"
"planning/07-10.robot-arm"
"planning/07-10.robot-arm"
"planning/07-10.robot-arm"
"planning/07-10.robot-arm"
"planning/11-12.sym-planner"
"planning/11-12.sym-planner"
"control/13.dmp"
"control/14.mpc"
"control/15.cem"
"control/16.bo"
)

MATCHING_IDXS=()

for i in "${!ALL_BENCHMARKS[@]}"; do
    BENCH_NAME="${ALL_BENCHMARKS[$i]}"
    BENCH_PATH="${ALL_PATHS[$i]}"
    if [[ "$BENCH_NAME" =~ $ARG ]] || [[ "$BENCH_PATH" =~ $ARG ]]; then
        MATCHING_IDXS+=( $i )
    fi
done

ROOT="$( readlink -f "$( dirname "${BASH_ROOT[0]}" )" )"

for IDX in "${MATCHING_IDXS[@]}"; do
    BENCH_NAME="${ALL_BENCHMARKS[$IDX]}"
    BENCH_PATH="${ROOT}/${ALL_PATHS[$IDX]}"

    echo "Building $BENCH_PATH"

    cd "$BENCH_PATH" || { echo "Could not cd to $BENCH_PATH"; continue; }
    if [[ ! -f "Makefile" ]]; then
        echo "Could not find Makefile in $BENCH_PATH"
    fi

    make "$FLAGS" > build.log 2>&1

    if [[ $? != 0 ]]; then
        echo "Error in compiling $BENCH_PATH"
    fi

    cd - > /dev/null || true
done

cd "$INIT_PATH" || true

exit 0
