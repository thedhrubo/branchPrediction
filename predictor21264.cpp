// A very stupid predictor.  It will always predict not taken.
#include "predictor21264.h"

unsigned globalHistoryTable = 0;


std::vector<unsigned> localHistoryTable(1023, 0);
std::vector<unsigned int> localPredictionTable(1023, 0);
std::vector<unsigned int> globalPredictionTable(4095, 0);
std::vector<unsigned int> chooserPredictionTable(4095, 0);

void init_predictor() {


}

bool make_prediction(unsigned int pc) {
    unsigned short historyIndex = globalHistoryTable;
    if (chooserPredictionTable[historyIndex] >= 2) {
        return globalPrediction();
    } else {
        return localPrediction(pc);
    }
}

bool globalPrediction() {
    unsigned int index = globalHistoryTable;
    if (globalPredictionTable[index] <= 1) {
        //Not taken
        return false;
    } else {
        return true; //taken
    }
}

bool localPrediction(unsigned int pc) {
    unsigned short index = pc & 1023;
    unsigned short predictIndex;
    //Retrieve 10-bit local history and generate prediction index
    predictIndex = localHistoryTable[index];
    //Determine if branch is taken or not by looking at the Local Prediction saturation
    if (localPredictionTable[predictIndex] < 4) {
        //Assume not taken
        return false;
    } else return true; // this one is for taken
}

void train_predictor(unsigned int pc, bool outcome) {
    trainChoicePrediction(pc, outcome);
    trainLocalPrediction(pc, outcome);
    trainGlobalPrediction(outcome);
}

void trainChoicePrediction(unsigned int pc, bool outcome) {
    //Get last prediction of local and global predictors
    bool localprediction = localPrediction(pc);
    bool globalprediction = globalPrediction();
    //Cross check choices to actual outcome
    if (globalprediction != localprediction) {
        if (localprediction == outcome) {
            if (chooserPredictionTable[globalHistoryTable] > 0)
                chooserPredictionTable[globalHistoryTable]--;
        } else if (globalprediction == outcome) {
            if (chooserPredictionTable[globalHistoryTable] < 3)
                chooserPredictionTable[globalHistoryTable]++;
        }
    }
}

void trainLocalPrediction(unsigned int pc, bool outcome) {
    unsigned short index = pc & 1023;
    unsigned short predictIndex;
    if (index < 100) {
        int i = 0;
    }
    predictIndex = localHistoryTable[index];
    if (outcome) {
        if (localPredictionTable[predictIndex] < 7)
            localPredictionTable[predictIndex]++;
    } else {
        if (localPredictionTable[predictIndex] > 0)
            localPredictionTable[predictIndex]--;
    }
    unsigned int check = localHistoryTable[index];

    if (localHistoryTable[index] != 1023)
        localHistoryTable[index] = localHistoryTable[index] << 1;

    if (localHistoryTable[index] > 1023) {
        localHistoryTable[index] = localHistoryTable[index] - 1024;
        if (outcome)
            localHistoryTable[index]++;
    } else if (localHistoryTable[index] == 1023) {
        localHistoryTable[index] = localHistoryTable[index] - 1;
        if (outcome)
            localHistoryTable[index]++;
    } else {
        if (outcome)
            localHistoryTable[index]++;
    }

}

void trainGlobalPrediction(bool outcome) {
    if (globalHistoryTable != 4095)
        globalHistoryTable = globalHistoryTable << 1;
    if (globalHistoryTable > 4095) {
        globalHistoryTable = globalHistoryTable - 4096;
        if (outcome)
            globalHistoryTable++;
    } else if (globalHistoryTable == 4095) {
        globalHistoryTable = globalHistoryTable - 1;
        if (outcome)
            globalHistoryTable++;
    } else {
        if (outcome)
            globalHistoryTable++;
    }

    if (globalHistoryTable < 128) {
        globalHistoryTable = globalHistoryTable << 1;
        if (outcome)
            globalHistoryTable++;
    }
}


