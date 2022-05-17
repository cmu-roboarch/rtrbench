#! /usr/bin/env bash

INPUT_SET="living-room"
LINK="https://cmu.box.com/shared/static/viydksoj273imvg24w6png5h4wutt03y"

mkdir -p "$INPUT_SET"
curl -L -C - "$LINK" -o "$INPUT_SET".tar.gz &> /dev/null
tar xf "$INPUT_SET".tar.gz -C "$INPUT_SET" &> /dev/null
rm "$INPUT_SET".tar.gz

echo "Downloaded! Check: $INPUT_SET"
