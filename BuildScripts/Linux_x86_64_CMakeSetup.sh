#!bash
# -- bash must be in the PATH env var

echo "The following must be the root folder of this project:"
pwd
echo "======================================================"
echo "cmake, gcc, and g++ MUST be on environment PATH"
echo "Ninja generator must also be installed"
echo "======================================================"

echo "\n"

echo "Running CMake command to set up debug"
cmake -DCMAKE_BUILD_TYPE=Debug -G "Ninja" -B cmake-build-debug-x64_linux --compile-no-warning-as-error --toolchain "./ToolChains/Linux_x86-64_Toolchain.cmake"

echo "Running CMake command to set up rlease"
cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" -B cmake-build-debug-x64_linux --compile-no-warning-as-error --toolchain "./ToolChains/Linux_x86-64_Toolchain.cmake"
