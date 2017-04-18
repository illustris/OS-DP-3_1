#include "simulator.h"
#include "scheduler.h"

#include <iostream>
#include <limits.h>
#include <algorithm>

int slack(job x)
{
	int slk = x.Deadline-CurrentTime-x.ExecutionTime[x.Criticality-1];
	cout<<"[DBG] Slack at start of Task "<<x.ID<<" = "<<slk<<endl;
	int xID=x.ID;
	//sort(ArrivedTaskList.begin(),ArrivedTaskList.end(),arrivaltime());
	for(int i=0;i<TaskCount;i++)
	{
		if(xID == TaskList[i].ID || TaskList[i].ArrivalTime > x.Deadline)
			continue;
		if(TaskList[i].Deadline > x.Deadline)
		{
			slk -= (int)((float)x.ExecutionTime[x.Criticality-1] *
				(float)(x.Deadline - max(TaskList[i].ArrivalTime,x.ArrivalTime)) /
					(float)(TaskList[i].Deadline - TaskList[i].ArrivalTime));
			cout<<"[DBG] slk -= "<<x.ExecutionTime[x.Criticality-1] <<"*("<<x.Deadline <<"-"<< max(TaskList[i].ArrivalTime,x.ArrivalTime)<<")"<<"/"<<(TaskList[i].Deadline - TaskList[i].ArrivalTime)<<endl;
		}
	}
	cout<<"[DBG] returning slk = "<<slk<<endl;
	return slk;
	//return x.Deadline;
}

struct LST
{
	inline bool operator() (const job& a, const job& b)
	{
		return (slack(a) < slack(b));
	}
};

struct crit
{
	inline bool operator() (const job& a, const job& b)
	{
		return (a.Criticality < b.Criticality);
	}
};

struct EDF
{
	inline bool operator() (const job& a, const job& b)
	{
		return (a.Deadline < b.Deadline);
	}
};

struct arrivaltime
{
	inline bool operator() (const job& a, const job& b)
	{
		return (a.ArrivalTime < b.ArrivalTime);
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
				<<" due to deadline miss\n";
			ArrivedTaskList.erase(ArrivedTaskList.begin());
		}
		else
			break;
	}
	sort(ArrivedTaskList.begin(),ArrivedTaskList.end(),LST());
	CurrentJob=&ArrivedTaskList[0];
	if(slack(ArrivedTaskList[0]) == 0)
	{
		cout<<"[Sched] Scheduling Task "<<ArrivedTaskList[0].ID<<" with slack "<<slack(ArrivedTaskList[0])<<endl;
		return CurrentTime+(*CurrentJob).RealExecutionTime;
	}
	else
	{
		cout<<"[Sched] Scheduling aperiodic server for "<<slack(ArrivedTaskList[0])<<"\n";
		CurrentJob=&AperiodicServer;
		return slack(ArrivedTaskList[0])+CurrentTime;
	}
}