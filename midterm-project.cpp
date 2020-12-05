#include <iostream>
#include <cmath>
using namespace std;

//declare structures
struct Task
{
  int id;
  int quantity;
  int deadline;
};
struct Machine
{
  int id;
  double idealProduction;
  double initialProduction;
  double decreasingRate;
  double minimumProduction;
  int maintenanceTime;
  double curPeriod; // starts from 1
  int calcTime(Task aTask, int repairOrNot); // if no repair: repairOrNot = 0
};

double createYield(int whichPeriodToRepair, double stdOutput, double yieldDec, int repairPeriods, double initYield, int curPeriod,
                   double lowestYieldConstraint);

//declare global variables
int machineNum = -1, taskNum = -1, maxRepairNum = -1;
Machine *machine;
Task *task;

//algorithm
int main()
{
  //begin input
  cin >> machineNum >> taskNum >> maxRepairNum;

  machine = new Machine[machineNum];

  for (int i = 0; i < machineNum; i++)
  {
    machine[i].id = i;
    cin >> machine[i].idealProduction >> machine[i].decreasingRate >> machine[i].maintenanceTime >> machine[i].initialProduction >> machine[i].minimumProduction;
  }

  task = new Task[taskNum];

  for (int i = 0; i < taskNum; i++)
  {
    task[i].id = i;
    cin >> task[i].quantity >> task[i].deadline;
  }
  //end of input

  return 0;
}

int Machine::calcTime(Task aTask, int repairOrNot) {
    int quantity = aTask.quantity, currentPeriod = curPeriod, consumedPeriod = 0;
    double lowestYieldConstraint = idealProduction * minimumProduction / static_cast<float>(100);
    while (true) {
        if (quantity <= 0) {
            return consumedPeriod;
        }
        quantity -= createYield(repairOrNot, idealProduction, decreasingRate, maintenanceTime, initialProduction, currentPeriod, lowestYieldConstraint);
        consumedPeriod++;
        currentPeriod++;
    }
}

double createYield(int whichPeriodToRepair, double stdOutput, double yieldDec, int repairPeriods, double initYield, int curPeriod,
                   double lowestYieldConstraint) {
    if (whichPeriodToRepair == 0) { // do not repair
        return max(stdOutput * round(initYield - yieldDec * (curPeriod)) / static_cast<float>(100),
                   lowestYieldConstraint);
    }
    else if (whichPeriodToRepair == 1) {
        if (whichPeriodToRepair <= curPeriod && curPeriod <= whichPeriodToRepair + repairPeriods - 1) {
            return 0;
        }
        else {
            return max(stdOutput * round(100.0 - yieldDec * (curPeriod - whichPeriodToRepair - repairPeriods)) / static_cast<float>(100),
                       lowestYieldConstraint);
        }
    }
    else {
        cout << "You can only repair at the first period or do not repair at all. Input 1 or 0 instead." << endl;
        return 0;
    }
}
