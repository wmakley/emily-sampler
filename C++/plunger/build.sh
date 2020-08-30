#!/bin/sh
cmake --build "$(pwd)"/build --config Debug --target all -- -j 6
