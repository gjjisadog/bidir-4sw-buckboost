#!/bin/bash
#
# Flash script for F28335 using TI DSLite
# Usage: ./flash.sh [hex_file]
#

# Default hex file
HEX_FILE="${1:-build/bin/buckboost_f28335.hex}"

# Check if DSLITE is set
if [ -z "$DSLITE" ]; then
    echo "Error: DSLITE environment variable not set"
    echo "Set it to: export DSLITE=/path/to/ccs_base/common/uscif/dslite"
    exit 1
fi

# Check if CCXML is set
if [ -z "$CCXML" ]; then
    echo "Error: CCXML environment variable not set"
    echo "Set it to: export CCXML=/path/to/F28335.ccxml"
    exit 1
fi

# Check if hex file exists
if [ ! -f "$HEX_FILE" ]; then
    echo "Error: Hex file not found: $HEX_FILE"
    echo "Please build the project first: make"
    exit 1
fi

# Flash the device
echo "Flashing $HEX_FILE to F28335..."
"$DSLITE" flash -e -f "$HEX_FILE" -c "$CCXML"

if [ $? -eq 0 ]; then
    echo "Flash successful!"
else
    echo "Flash failed!"
    exit 1
fi


