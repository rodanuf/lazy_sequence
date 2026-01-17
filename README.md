how to install:

in cmd/bash: 

git clone --recurse-submodules https://github.com/rodanuf/lazy_sequence.git
cd lazy_sequence


how to build:

choose lazy_sequence dir, then create build dir (mkdir build), move there, execute  cmake .. -DCMAKE_BUILD_TYPE=Release for unix or cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" if you use w64devkit
else -> cmake .. -DCMAKE_CXX_COMPILER=your_compiler -DCMAKE_BUILD_TYPE=Debug

after that use cmake --build .

And there will two executable files: tests.exe and lab.exe. tests.exe - tests for project, lab.exe - main project.
