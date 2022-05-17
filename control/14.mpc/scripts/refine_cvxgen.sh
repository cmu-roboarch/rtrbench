#! /usr/bin/env bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of arguments"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "Couldn't find directory $1"
    exit 1
fi

cd $1

rm *.m
rm csolve.c
rm Makefile
sed -i 's/^int main/\/\/ int main/g' solver.h

for file in *.c; do
    NAME=$(basename $file .c)
    mv $file $NAME.cpp
done

mv description.cvxgen ldl.cpp matrix_support.cpp solver.* util.cpp ../

cd .. || true

exit 0
