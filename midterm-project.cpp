#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

//declare structures
struct Task
{
  int id;
  int quantity;
  int deadline;
  int score = 0;
};
struct Machine
{
  int id;
  double idealProduction;
  double initialProduction;
  double decreasingRate;
  double minimumProduction;
  int maintenanceTime;
  double currentPeriod;                          // starts from 1
  int calcTime(Task testTask, bool repairOrNot); // if no repair: repairOrNot = 0
};

double createYield(int whichPeriodToRepair, double stdOutput, double yieldDec, int repairPeriods, double initYield, int currentPeriod, double lowestYieldConstraint);

//declare global variables
int machineNum = -1, taskNum = -1, maxRepairNum = -1;
Machine *machine;
Task *task;

//declare functions
void sortTasks();

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

void sortTasks()
{
  //multipliers
  const int deadlineMultiplier = 1;
  const int quantityMultiplier = 1;

  //sort
  sort(task, task + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.deadline < rhs.deadline;
  });

  for (int i = taskNum; i > 0; i--)
  {
    task[i].score += i * deadlineMultiplier;
  }

  sort(task, task + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.quantity < rhs.quantity;
  });

  for (int i = taskNum; i > 0; i--)
  {
    task[i].score += i * quantityMultiplier;
  }

  sort(task, task + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.score > rhs.score;
  });
}

int Machine::calcTime(const Task testTask, const bool repairOrNot)
{
  int quantity = testTask.quantity, tmpCurrentPeriod = currentPeriod, consumedPeriod = 0;
  double lowestYieldConstraint = idealProduction * minimumProduction / static_cast<double>(100);
  while (true)
  {
    if (quantity <= 0)
    {
      return consumedPeriod;
    }
    quantity -= createYield(repairOrNot, idealProduction, decreasingRate, maintenanceTime, initialProduction, tmpCurrentPeriod, lowestYieldConstraint);
    consumedPeriod++;
    tmpCurrentPeriod++;
  }
}

double createYield(int whichPeriodToRepair, double stdOutput, double yieldDec, int repairPeriods, double initYield, int currentPeriod, double lowestYieldConstraint)
{
  if (whichPeriodToRepair == 0)
  { // do not repair
    return max(stdOutput * round(initYield - yieldDec * (currentPeriod)) / static_cast<double>(100),
               lowestYieldConstraint);
  }
  else if (whichPeriodToRepair == 1)
  {
    if (whichPeriodToRepair <= currentPeriod && currentPeriod <= whichPeriodToRepair + repairPeriods - 1)
    {
      return 0;
    }
    else
    {
      return max(stdOutput * round(100.0 - yieldDec * (currentPeriod - whichPeriodToRepair - repairPeriods)) / static_cast<double>(100),
                 lowestYieldConstraint);
    }
  }
  else
  {
    cout << "You can only repair at the first period or do not repair at all. Input 1 or 0 instead." << endl;
    return 0;
  }
}
