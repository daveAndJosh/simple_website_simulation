#!/bin/sh

# Tell user they must have cadmium_v2 ready
echo "You must have cadmium_v2 installed in a sibling directory to this simulation"
echo "It also must be built. If this is not the case, the simulation will not work"

# make logs directory
mkdir logs
# setup with cmake and make
mkdir build
cd build
cmake ..
make
cd ..
# run
cd bin
./test
./serverless_recipe_site_sim
cd ..
echo "all tests ran and main"
echo "find outputs in log folder"
