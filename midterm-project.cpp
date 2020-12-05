#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

//multipliers
const float durationMultiplier = 2;
const float endTimeMultiplier = 3;
const float deadlineMultiplier = 2;
const float quantityMultiplier = 3;

//declare structures
struct Task
{
  int id = 0;
  int quantity = 0;
  int deadline = 0;
  int score = 0;
};
struct Machine
{
  int id = 0;
  double idealProduction = 0;
  double initialProduction = 0;
  double decreasingRate = 0;
  double minimumProduction = 0;
  int maintenanceTime = 0;
  int currentPeriod = 0;                         // starts from 1
  int calcTime(Task testTask, bool repairOrNot); // if no repair: repairOrNot = 0
};
struct Assignment
{
  int time = 0;
  bool repair = 0;
  int assignTo = 0;
  int score = 0;
};
struct RepairTimeSet
{
  int begin;
  int end;
};

//declare global variables
int machineNum = -1, taskNum = -1, maxRepairNum = -1;
Machine *machine = nullptr;
Task *tasks = nullptr;
Assignment *assignments = nullptr;
int maxDeadline = 0;
vector<RepairTimeSet> repairLog;

//declare functions
void sortTasks();
double createYield(int whichPeriodToRepair, double stdOutput, double yieldDec, int repairPeriods, double initYield, int currentPeriod, double lowestYieldConstraint);
Assignment optimalAssignment(Task);
int calculateScore(int, int);
void sortAssignment();
void writeRepair(int, int);
bool checkRepair(int, int);

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
  assignments = new Assignment[(machineNum * 2)];
  repairLog.clear();
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
    writeRepair(machine[optimal.assignTo].currentPeriod, machine[optimal.assignTo].maintenanceTime);
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
          cout << schedule[i][j] + 1;
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
  delete[] assignments;
  return 0;
}

void sortTasks()
{
  //sort

  sort(tasks, tasks + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.quantity < rhs.quantity;
  });

  for (int i = 0; i < taskNum; i++)
  {
    tasks[i].score += (taskNum - i + 1) * quantityMultiplier;
  }

  sort(tasks, tasks + taskNum, [](const Task &lhs, const Task &rhs) {
    return lhs.deadline < rhs.deadline;
  });

  for (int i = 0; i < taskNum; i++)
  {
    tasks[i].score += (taskNum - i + 1) * deadlineMultiplier;
  }

  sort(tasks, tasks + taskNum, [](const Task &lhs, const Task &rhs) {
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

Assignment optimalAssignment(const Task task)
{
  for (int i = 0; i < machineNum; i++)
  {
    assignments[i * 2].time = machine[i].calcTime(task, 0);
    assignments[i * 2].assignTo = i;
    assignments[i * 2].repair = 0;

    if (checkRepair(machine[i].currentPeriod, machine[i].maintenanceTime))
    {
      assignments[i * 2 + 1].time = machine[i].calcTime(task, 1);
      assignments[i * 2 + 1].assignTo = i;
      assignments[i * 2 + 1].repair = 1;
    }
  }
  sortAssignment();
  Assignment optimal;
  optimal.time = assignments[0].time;
  optimal.assignTo = assignments[0].assignTo;
  optimal.repair = assignments[0].repair;
  return optimal;
}

void sortAssignment()
{
  {

    for (int i = 0; i < machineNum * 2; i++)
    {
      assignments[i].score = 0;
    }

    //sort
    sort(assignments, assignments + machineNum * 2, [](const Assignment &lhs, const Assignment &rhs) {
      return lhs.time < rhs.time;
    });

    for (int i = 0; i < machineNum * 2; i++)
    {
      assignments[i].score += (machineNum * 2 - i + 1) * durationMultiplier;
    }

    sort(assignments, assignments + machineNum * 2, [](const Assignment &lhs, const Assignment &rhs) {
      return lhs.time + machine[lhs.assignTo].currentPeriod < rhs.time + machine[rhs.assignTo].currentPeriod;
    });

    for (int i = 0; i < machineNum * 2; i++)
    {
      assignments[i].score += (machineNum * 2 - i + 1) * endTimeMultiplier;
    }

    sort(assignments, assignments + machineNum * 2, [](const Assignment &lhs, const Assignment &rhs) {
      return lhs.score > rhs.score;
    });
  }
}

void writeRepair(int start, int duration)
{
  RepairTimeSet tmp;
  tmp.begin = start;
  tmp.end = start + duration - 1;
  repairLog.push_back(tmp);
}

bool checkRepair(int start, int duration)
{
  bool repairable = 1;
  for (int i = start; i < start + duration - 1; i++)
  {
    int repairCnt = 0;
    for (int j = 0; j < repairLog.size(); j++)
    {
      if (i >= repairLog[j].begin && i <= repairLog[j].end)
      {
        repairCnt++;
      }
      if (repairCnt >= maxRepairNum)
      {
        repairable = 0;
        break;
      }
    }
    if (repairCnt >= maxRepairNum)
    {
      repairable = 0;
    }
    if (!repairable)
    {
      break;
    }
  }
  return repairable;
}
