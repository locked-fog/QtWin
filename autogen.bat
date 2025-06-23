rmdir /s /q build
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=./qtwin/
cmake --build .
cmake --install .
cpack -G ZIP