#include <iostream>
#include <algorithm>
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
};

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
