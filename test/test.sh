#!/bin/bash

echo ""
echo "====== Execute Pitcher with default settings ======"
echo ""
./Pitcher -i processF0List

echo ""
echo "====== Execute ProFeXor with default settings ======"
echo ""
./ProFeXor -S PSDEX -D -m test.rec -d def.txt
