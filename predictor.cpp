// A very stupid predictor.  It will always predict not taken.
#include "predictor.h"

static const int branchHistoryTable = 16383;
unsigned long long globalRegister = 0;

unsigned int globalRegisterSize = (int) pow(2, 14) - 1;
std::vector<unsigned int> counter(branchHistoryTable, 0);

void init_predictor() {


}

bool make_prediction(unsigned int pc) {
    int index = (pc ^ (globalRegister & globalRegisterSize)) % branchHistoryTable;
    unsigned int state = counter[index];
    if (state == 0 || state == 1) {
        return false;
    } else return true;
    }

void train_predictor(unsigned int pc, bool outcome) {
    int index = (pc ^ (globalRegister & globalRegisterSize)) % branchHistoryTable;
    unsigned int state = counter[index];
    if (outcome) {
        if (state != 3)
            state++;
    } else {
        if (state != 0)
            state--;
    }
    globalRegister = globalRegister << 1 | outcome;
    counter[index] = state;
}
