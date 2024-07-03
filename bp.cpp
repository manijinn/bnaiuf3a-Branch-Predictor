#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <cstdio>
using namespace std;

// I am using a hash table to implement a pseudo branch history table.
unordered_map<unsigned long long int, int> BHT;
int M;
int N;
unsigned long long int GBR = 0;
int miss = 0;

// Gshare procdure.
static unsigned long long int getIndex(unsigned long long int add) {
    unsigned long long int bitmask = (1 << M) - 1;
    add = add >> 2;
    add = add & bitmask;

    return add ^ (GBR << (M - N));
}

static void updateGBR(char outcome) {
    // Shifting right by 1 constitutes the not taken scenario, and shifts a 0 into the register.
    GBR = GBR >> 1;

    // If outcome was taken, then we turn the 0 we just shifted into the register into a 1 instead.
    if (outcome == 't' && N != 0) {
        GBR = GBR | (1 << (N - 1));
    }
}

// Insert entries here.
static void insertEntry(unsigned long long int add, char outcome) {
    unsigned long long int index = getIndex(add);

    // If the key (index) was not found in the hash table, then we assume the key to store weakly taken.
    if (BHT.find(index) == BHT.end()) {

        if (outcome == 't') {
            BHT.insert({index, 0x3});
        }

        // Miss if outcome was not taken.
        else {
            BHT.insert({index, 0x1});
            miss++;
        }
    }

    // Key exists and already stores a previous prediction.
    else {
        unsigned long long int temp = BHT.at(index);

        if (outcome == 't') {

            // Prediction is not 03 (taken).
            if (temp <= 0x2) {

                // Prediction is not 02 (weakly taken) either. Fail, we increment misses.
                if (temp < 0x2) {
                    miss++;
                }
                // Since the key is not 3, we increment what is stored in it.
                BHT[index] += 1;
            }
        }

        else {

            // Prediction is not 00 (not taken).
            if (temp >= 0x1) {

                // Prediction is not 01 (weakly not taken). Fail, increment misses.
                if (temp > 0x1) {
                    miss++;
                }
                // Since the key is not 0, we decrement what is stored in it.
                BHT[index] -= 1;
            }
        }
    }

    updateGBR(outcome);
}

int main(int argc, char* argv[]) {
    // Get command line arguments
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    string TRACE_FILE = argv[3];

    // Get file and open it
    const char* charArray = TRACE_FILE.c_str();
    FILE *file = fopen(charArray, "r"); 

    unsigned long long int add;
    char outcome;
    int add_num = 0;

    // If file exists
    if (file) {
        
        while (fscanf(file, " %llx %c" , &add, &outcome)) {

            if (feof(file)) {
                fclose(file);
                break;
            }
            insertEntry(add, outcome);
            add_num++;
        }
    }

    else {
        cout << "The file you inputed was not found!" << endl;
    }

    printf("%d %d %f%%\n", M, N, (float)(((float)miss / (float)add_num) * 100));
    BHT.clear();
    fflush(stdout);
}