#include "simulator.h"
#include "scheduler.h"

#include <iostream>
#include <limits.h>
#include <algorithm>

struct EDF
{
	inline bool operator() (const job& a, const job& b)
	{
		return (a.Deadline < b.Deadline);
	}
};

int Scheduler() //return end time of current task or INT_MAX if nothing to be done
{
	//run acceptance test, schedule
	cout<<"[Sched] Called at "<<CurrentTime<<"\n";
	if(ArrivedTaskList.size()==0)
	{
		SchedDone=1;
		return INT_MAX;
	}
	sort(ArrivedTaskList.begin(),ArrivedTaskList.end(),EDF());
	while(ArrivedTaskList.size()>0)
	{
		if(ArrivedTaskList[0].Deadline<ArrivedTaskList[0].RealExecutionTime+CurrentTime)
		{
			cout<<"[Sched] Discarded Task "<<ArrivedTaskList[0].ID
				<<"due to deadline miss\n";
			ArrivedTaskList.erase(ArrivedTaskList.begin());
		}
		else
			break;
	}
	CurrentJob=&ArrivedTaskList[0];
	return CurrentTime+(*CurrentJob).RealExecutionTime;
}