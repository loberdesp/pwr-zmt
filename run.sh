rm -rf build
mkdir build
cd build
cmake ../
cmake --build .
./cmds_interpreter ../test.txt