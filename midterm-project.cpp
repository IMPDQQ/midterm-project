#include <iostream>
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
};

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
