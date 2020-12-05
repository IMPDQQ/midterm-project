#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
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
struct Assignment
{
  int time;
  bool repair;
  int assignTo;
  int score = 0;
};

//declare global variables
int machineNum = -1, taskNum = -1, maxRepairNum = -1;
Machine *machine = nullptr;
Task *tasks = nullptr;
Assignment *assignments = nullptr;

//declare functions
void sortTasks();
double createYield(int whichPeriodToRepair, double stdOutput, double yieldDec, int repairPeriods, double initYield, int currentPeriod, double lowestYieldConstraint);
Assignment optimalAssignment(Task);
int calculateScore(int, int);
void sortAssignment();

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

  tasks = new Task[taskNum];

  for (int i = 0; i < taskNum; i++)
  {
    tasks[i].id = i;
    cin >> tasks[i].quantity >> tasks[i].deadline;
  }
  //end of input

  sortTasks();

  vector<int> schedule[machineNum];

  for (int i = 0; i < taskNum; i++)
  {
    Assignment optimal = optimalAssignment(tasks[i]);
    if (optimal.repair)
    {
      schedule[optimal.assignTo].push_back(-1);
    }
    schedule[optimal.assignTo].push_back(i);
    machine[optimal.assignTo].currentPeriod += optimal.time;
  }

  for (int i = 0; i < machineNum; i++)
  {
    if (schedule[i].size() == 0)
    {
      cout << "0\n";
    }
    else
    {
      for (int j = 0; j < schedule[i].size(); j++)
      {
        if (j != 0)
        {
          cout << ",";
        }
        if (schedule[i][j] == -1)
        {
          cout << "M";
        }
        else
        {
          cout << schedule[i][j];
        }
      }
      if (i != machineNum - 1)
      {
        cout << "\n";
      }
    }
  }

  delete[] machine;
  delete[] tasks;
  return 0;
}

void sortTasks()
{
  //multipliers
  const int deadlineMultiplier = 1;
  const int quantityMultiplier = 1;

  //sort
  sort(tasks, tasks + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.deadline < rhs.deadline;
  });

  for (int i = taskNum; i > 0; i--)
  {
    tasks[i].score += i * deadlineMultiplier;
  }

  sort(tasks, tasks + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.quantity < rhs.quantity;
  });

  for (int i = taskNum; i > 0; i--)
  {
    tasks[i].score += i * quantityMultiplier;
  }

  sort(tasks, tasks + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.score < rhs.score;
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
    return max(stdOutput * round(initYield - yieldDec * (currentPeriod)) / static_cast<double>(100), lowestYieldConstraint);
  }
  else if (whichPeriodToRepair == 1)
  {
    if (whichPeriodToRepair <= currentPeriod && currentPeriod <= whichPeriodToRepair + repairPeriods - 1)
    {
      return 0;
    }
    else
    {
      return max(stdOutput * round(100.0 - yieldDec * (currentPeriod - whichPeriodToRepair - repairPeriods)) / static_cast<double>(100), lowestYieldConstraint);
    }
  }
  else
  {
    cout << "You can only repair at the first period or do not repair at all. Input 1 or 0 instead." << endl;
    return 0;
  }
}

Assignment optimalAssignment(Task task)
{
  assignments = new Assignment[machineNum * 2];
  cout << "flag";
  for (int i = 0; i < machineNum; i++)
  {
    assignments[i * 2].time = machine[i].calcTime(task, 0);
    assignments[i * 2].assignTo = i;
    assignments[i * 2].repair = 0;
    assignments[i * 2 + 1].time = machine[i].calcTime(task, 1);
    assignments[i * 2 + 1].assignTo = i;
    assignments[i * 2 + 1].repair = 1;
  }
  sortAssignment();
  Assignment optimal;
  optimal.time = assignments[0].time;
  optimal.assignTo = assignments[0].assignTo;
  optimal.repair = assignments[0].repair;
  delete[] assignments;
  return optimal;
}

void sortAssignment()
{
  {
    //multipliers
    const int durationMultiplier = 1;
    const int endTimeMultiplier = 1;

    //sort
    sort(assignments, assignments + machineNum * 2, [](const Assignment &lhs, const Assignment &rhs) {
      return lhs.time < rhs.time;
    });

    for (int i = machineNum * 2; i > 0; i--)
    {
      assignments[i].score += i * durationMultiplier;
    }

    sort(assignments, assignments + machineNum * 2, [](const Assignment &lhs, const Assignment &rhs) {
      return lhs.time + machine[lhs.assignTo].currentPeriod < rhs.time + machine[rhs.assignTo].currentPeriod;
    });

    for (int i = machineNum * 2; i > 0; i--)
    {
      assignments[i].score += i * endTimeMultiplier;
    }

    sort(assignments, assignments + machineNum * 2, [](const Assignment &lhs, const Assignment &rhs) {
      return lhs.score < rhs.score;
    });
  }
}
