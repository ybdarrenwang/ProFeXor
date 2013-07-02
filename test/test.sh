#!/bin/bash

echo ""
echo "====== Execute Pitcher with default settings ======"
echo ""
./Pitcher -i f0List -o processedF0

echo ""
echo "====== Execute ProFeXor with default settings ======"
echo ""
./ProFeXor -S PSDEX -D -m test.rec -d def.txt
