#include<iostream>
using namespace std;

//declare structures
const int MAX_ORDERS = ;
struct Task {
  int id;
  int quantity;
  int deadline;
};
struct Machine {
  int id;
  double idealProduction;
  double initialProduction;
  double decreasingRate;
  double minimumProduction;
  int maintenanceTime;
};
double calcTime(Task aTask, Machine aMachine, bool repairOrNot) {
    return 0;
}
float max(float a, float b);
int min(int a, int b);
float myRound(float num);
void mergeSort(int orderCount, int** array, int l, int r);
void merge(int orderCount, int** array, int l , int r);
float* createYieldArray(int whichPeriodToRepair, float stdOutput, float yieldDec, int repairPeriods, float initYield,
                        int totalPeriods, float lowestYieldConstraint);
int* orderFinishedPeriod(const int orderQty[], const float yieldArrayWithRepair[], int orderCount, int totalPeriods);
int arrayElementDiff(const int arrayA[], const int arrayB[], int arrayLength);
int* possibleRepairPeriods(const int orderQty[], const float yieldArrayWithRepair[], int orderCount, int totalPeriods);
void printWhenToRepair(float stdOutput, float yieldDec, int repairPeriods, float initYield, int totalPeriods,
                       float lowestYieldConstraint, const int orderQty[], int orderCount, int deliverTime[]);
int main() {
    float stdOutput = 0, yieldDec = 0, initYield = 0, yieldLowerBound = 0;
    int repairPeriods = 0, totalPeriods, orderCount = 0;
    cin >> stdOutput >> yieldDec >> repairPeriods >> initYield >> totalPeriods >> yieldLowerBound >> orderCount;
    float lowestYieldConstraint = stdOutput * yieldLowerBound / static_cast<float>(100);
    int* orderQty = new int[orderCount];
    int* deliverTime = new int[orderCount];
    for (int i = 0; i < orderCount; i++) {
        cin >> orderQty[i];
    }
    for (int i = 0; i < orderCount; i++) {
        cin >> deliverTime[i];
    }

    int** arrayToSort = new int*[orderCount];
    for (int i = 0; i < orderCount; i++) {
        arrayToSort[i] = new int[3];
        arrayToSort[i][0] = deliverTime[i];
        arrayToSort[i][1] = orderQty[i];
        arrayToSort[i][2] = i;
    }

    mergeSort(orderCount, arrayToSort, 0, orderCount - 1);
    int sortedOrderQty[MAX_ORDERS] = {0};
    for (int i = 0; i < orderCount; i++) {
        sortedOrderQty[i] = arrayToSort[i][1];
    }
    int sortedDeliverTime[MAX_ORDERS] = {0};
    for (int i = 0; i < orderCount; i++) {
        sortedDeliverTime[i] = arrayToSort[i][0];
    }
    printWhenToRepair(stdOutput, yieldDec, repairPeriods, initYield, totalPeriods, lowestYieldConstraint, sortedOrderQty, orderCount, sortedDeliverTime);
}

float max(float a, float b) {
    if (b >= a) {
        return b;
    }
    return a;
}

int min(int a, int b) {
    if (a >= b)
        return b;
    else
        return a;
}

float myRound(float num) {
    int upper = static_cast<int>(num + 1);
    int lower = static_cast<int>(num);
    if (static_cast<float>(upper) - num >= num - static_cast<float>(lower)) {
        return lower;
    }
    return upper;
}

void mergeSort(int orderCount, int** array, int l, int r) {
    if (l >= r) {
        return;
    }
    int med = (l + r) / 2;
    mergeSort(orderCount, array, l, med);
    mergeSort(orderCount, array, med + 1, r);
    merge(orderCount, array, l, r);
}

void merge(int orderCount, int** array, int l, int r) {
    int** temp = new int*[r - l + 1];
    for (int i = 0; i < r - l + 1; i++) {
        temp[i] = new int[3];
    }
    // length of this section: r - l + 1
    int medium = (l + r) / 2;
    // section A: array[0:medium + 1], sorted
    // section B: array[medium + 1:r + 1], sorted
    int secACount = l, secBCount = medium + 1, resultIndex = 0;
    while (secACount <= medium && secBCount <= r) {
        if (array[secACount][0] < array[secBCount][0] ||
            (array[secACount][0] == array[secBCount][0] && array[secACount][1] < array[secBCount][1]) ||
            (array[secACount][0] == array[secBCount][0] && array[secACount][1] == array[secBCount][1] && array[secACount][2] <= array[secBCount][2])) {
            for (int i = 0; i < 3; i++) {
                temp[resultIndex][i] = array[secACount][i];
            }
            secACount++;
            resultIndex++;
        }
        else {
            for (int i = 0; i < 3; i++) {
                temp[resultIndex][i] = array[secBCount][i];
            }
            secBCount++;
            resultIndex++;
        }
    }
    for (int i = secACount; i < medium + 1; i++) {
        for (int j = 0; j < 3; j++) {
            temp[resultIndex][j] = array[i][j];
        }
        resultIndex++;
    }
    for (int i = secBCount; i < r + 1; i++) {
        for (int j = 0; j < 3; j++) {
            temp[resultIndex][j] = array[i][j];
        }
        resultIndex++;
    }
    for (int i = l; i <= r; i++) {
        for (int j = 0; j < 3; j++) {
            array[i][j] = temp[i - l][j];
        }
    }
    delete [] temp;
}

float* createYieldArray(int whichPeriodToRepair, float stdOutput, float yieldDec, int repairPeriods, float initYield, int totalPeriods,
                        float lowestYieldConstraint) {
    float* yieldArray = new float[totalPeriods];
    if (whichPeriodToRepair != 0) {
        for (int j = 1; j < min(whichPeriodToRepair, totalPeriods + 1); j++) {
            yieldArray[j - 1] =
                    max(stdOutput * myRound(initYield - yieldDec * j) / static_cast<float>(100),
                        lowestYieldConstraint);
        }
        for (int j = whichPeriodToRepair; j < min(whichPeriodToRepair + repairPeriods, totalPeriods + 1); j++) {
            yieldArray[j - 1] = 0;
        }
        for (int j = whichPeriodToRepair + repairPeriods; j <= totalPeriods; j++) {
            yieldArray[j - 1] =
                    max(stdOutput * myRound(100.0 - yieldDec * (j - whichPeriodToRepair - repairPeriods)) / static_cast<float>(100),
                        lowestYieldConstraint);
        }
    }
    else {
        for (int i = 0; i < totalPeriods; i++) {
            yieldArray[i] = max(stdOutput * myRound(initYield - yieldDec * (i + 1)) / static_cast<float>(100),
                                lowestYieldConstraint);
        }
    }
    return yieldArray;
}

int* orderFinishedPeriod(const int orderQty[], const float yieldArrayWithRepair[], int orderCount, int totalPeriods) {
    int* finishedPeriod = new int[orderCount];
    float* orderQtyCopy = new float[orderCount];
    for (int i = 0; i < orderCount; i++) {
        orderQtyCopy[i] = orderQty[i];
    }
    int whichOrder = 0;
    for (int i = 0 ; i < totalPeriods && whichOrder < orderCount; i++) {
        orderQtyCopy[whichOrder] -= yieldArrayWithRepair[i];
        if (orderQtyCopy[whichOrder] <= 0.0) { // 第 whichOrder 訂單做完了
            finishedPeriod[whichOrder] = i + 1;
            whichOrder++;
        }
    }
    for (int i = whichOrder; i < orderCount; i++) {
        finishedPeriod[i] = totalPeriods * 2;
    }
    delete [] orderQtyCopy;
    return finishedPeriod;
}

int* possibleRepairPeriods(const int orderQty[], const float yieldArrayWithRepair[], int orderCount, int totalPeriods) {
    int* doNotRepairFinishTime = orderFinishedPeriod(orderQty, yieldArrayWithRepair, orderCount, totalPeriods);
    int* possibleRepairPeriods = new int[orderCount + 1];
    possibleRepairPeriods[0] = 0; // 不維修
    possibleRepairPeriods[1] = 1; // First period 就維修
    for (int i = 0; i < orderCount - 1; i++) {
        possibleRepairPeriods[i + 2] = doNotRepairFinishTime[i] + 1;
    }
    delete [] doNotRepairFinishTime;
    return possibleRepairPeriods;
}

int arrayElementDiff(const int standard[], const int toCompare[], int arrayLength) {
    int result = 0;
    for (int i = 0; i < arrayLength; i++) {
        if (standard[i] >= toCompare[i])
            continue;
        else
            result += toCompare[i] - standard[i];
    }
    return result;
}

void printWhenToRepair(float stdOutput, float yieldDec, int repairPeriods, float initYield, int totalPeriods,
                       float lowestYieldConstraint, const int orderQty[], int orderCount, int deliverTime[]) {
    float* doNotRepairYieldArray = createYieldArray(0, stdOutput, yieldDec, repairPeriods, initYield,
                                                    totalPeriods, lowestYieldConstraint);
    int* possibleRepairPeriod = possibleRepairPeriods(orderQty, doNotRepairYieldArray, orderCount, totalPeriods);
    int minDelayPeriods = INT_MAX, optimalRepairPeriod = 0;
    for (int i = 0; i < orderCount + 1; i++) {
        float* yieldArrayWithRepair = createYieldArray(possibleRepairPeriod[i], stdOutput, yieldDec, repairPeriods,
                                                       initYield, totalPeriods, lowestYieldConstraint);

        int* finishedPeriod = orderFinishedPeriod(orderQty, yieldArrayWithRepair, orderCount, totalPeriods);
        int delayPeriods = arrayElementDiff(deliverTime, finishedPeriod, orderCount);
        if  (delayPeriods < minDelayPeriods) {
            minDelayPeriods = delayPeriods;
            optimalRepairPeriod = possibleRepairPeriod[i];
        }
        delete [] yieldArrayWithRepair;
        delete [] finishedPeriod;
    }
    cout << optimalRepairPeriod << "," << minDelayPeriods;
    delete [] possibleRepairPeriod;
    delete [] doNotRepairYieldArray;
}

