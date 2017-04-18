#include <vector>

using namespace std;

struct job
{
	int ID;
	int ArrivalTime;
	int ExecutionTime[4];
	int Period;
	int RealExecutionTime; //
	int Criticality;
	int RealCriticality; //
	int Deadline;
	int ScheduledEnd;
	bool Arrived; //
	bool Done;
	bool operator < (const job& j) const
	{
		return (ArrivalTime < j.ArrivalTime);
	}
};

extern bool TasksDone;
extern bool SchedDone;

extern int NextTime;
extern int CurrentTime;

extern int TaskCount;
extern vector<job> TaskList;
extern vector<job> ArrivedTaskList;

extern job* CurrentJob;
extern job AperiodicServer;