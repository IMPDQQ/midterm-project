#include<iostream>
using namespace std;

//declare structures
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


int main() {

    int machineNum = -1, taskNum = -1, maxRepairNum = -1;

    cin>> machineNum>>taskNum>>maxRepairNum;

    Machine machine[machineNum] = {0};

    for(int i = 0; i < machineNum;, i++)
    {
        machine[i].id = i;
        cin >> machine[i].idealProduction >> machine.[i].decreasingRate
        >> machine.[i].maintenanceTime >> machine[i].initialProduction
         >> machine.[i].minimumProduction;
    }

    Task task[taskNum] = {0};

    for(int i = 0; i < taskNum;, i++)
    {
        task[i].id = i;
        cin >> task[i].quantity>> task[i].deadline;
    }

  return 0;
}
