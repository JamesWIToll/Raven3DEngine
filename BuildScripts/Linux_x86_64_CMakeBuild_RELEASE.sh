#!bash
# -- bash must be in the PATH env var

echo "The following must be the root folder of this project:"
pwd
echo "======================================================"
echo "cmake, gcc, and g++ MUST be on environment PATH"
echo "======================================================"


echo "Running CMake command to build release"
cmake --build cmake-build-release
