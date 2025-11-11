rm -rf build
mkdir build
cd build
mkdir config
cp ../config/config.xml ../config/config.xsd config/
cmake ..
cmake --build .
./cmds_interpreter ../test.txt