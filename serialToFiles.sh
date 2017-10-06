#!/bin/bash

TTY='/dev/ttyACM0'
TARGETDIR='/run/atmo/'


echo Running atmo exporter
mkdir -p "$TARGETDIR"

while [ true ]
do

if [ ! -c $TTY ]; then
    echo "BigClown not connected."
    sleep 4s
    continue
fi

cat $TTY | while read INPUT
do
    echo "INPUT:$INPUT"
    KEY=$(echo "$INPUT" | cut -f1 -d':'|grep -o '[A-Za-z0-9]*')
    VALUE=$(echo "$INPUT" | cut -f2 -d':');
    echo "Storing key: $KEY with value $VALUE";

    echo "$VALUE" > "$TARGETDIR/$KEY"
done

echo "BigClown disconnected"
sleep 1s
done

