echo This can take some time, there are over 30000 lines of code to compile in the libraries...
# clang++-7 -pthread -std=c++17 -o main main.cpp minimax.cpp
# ./main
git pull
make
./bin/main
