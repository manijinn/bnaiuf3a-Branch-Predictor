# bnaiuf3a-Branch-Predictor
A branch predictor, like the ones commonly used for pipelining in computer architecture. It uses GSHARE and can handle an input of 10+ million addresses in one single file.

Compile and Run:
g++ -o <name> bp.cpp

-The command for running the executable is the same as specified in the project instructions. 
(Or in other words, the standard for C++).

./a <GPB> <RB> <Trace_File>

The results of my program are <M> <N> <Misprediction rate>

GPB specifies the size of the Branch History Table (M) while RB specifies the size of the Global Branch Register (N)
