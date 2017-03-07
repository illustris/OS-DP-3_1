#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits.h>

#include "simulator.h"
#include "scheduler.h"

using namespace std;

bool TasksDone=0;
bool SchedDone=0;

int NextTime=0;
int CurrentTime;

int TaskCount;
vector<job> TaskList;
vector<job> ArrivedTaskList;

job* CurrentJob;

void ReadTasks(char* FileName)
{
	string buff;
	ifstream TaskListFile;
	TaskListFile.open(FileName);
	TaskListFile>>TaskCount;
	TaskList.resize(TaskCount);
	for(int i=0;i<TaskCount;i++)
	{
		TaskListFile>>TaskList[i].ArrivalTime;
		for(int j=0;j<4;j++)
			{
				TaskListFile>>TaskList[i].ExecutionTime[j];
			}
		TaskListFile>>TaskList[i].Criticality>>TaskList[i].RealCriticality
										>>TaskList[i].Deadline;
		TaskList[i].RealExecutionTime=TaskList[i].ExecutionTime[TaskList[i].RealCriticality-1];
		TaskList[i].ID=i;
	}
}

void PrintTasks()
{
	cout<<TaskList.size()<<" Tasks\n";
	for(int i=0;i<TaskCount;i++)
	{
		cout<<"Task "<<i+1<<"\nArrival: "<<TaskList[i].ArrivalTime
			<<"\nExecution times: ";
		for(int j=0;j<4;j++)
			cout<<TaskList[i].ExecutionTime[j]<<"\t";
		cout<<"\nCriticality: "<<TaskList[i].Criticality<<"\nDeadline: "
			<<TaskList[i].Deadline<<endl;
	}
	cout<<"\n";
}

/*void PrintATasks()
{
	cout<<ArrivedTaskList.size()<<" Tasks\n";
	for(int i=0;i<ArrivedTaskList.size();i++)
	{
		cout<<"Task "<<i+1<<"\nArrival: "<<ArrivedTaskList[i].ArrivalTime
			<<"\nExecution times: ";
		for(int j=0;j<4;j++)
			cout<<ArrivedTaskList[i].ExecutionTime[j]<<"\t";
		cout<<"\nCriticality: "<<ArrivedTaskList[i].Criticality<<"\nDeadline: "
			<<ArrivedTaskList[i].Deadline<<endl;
	}
	cout<<"\n";
}*/

void StartSim()
{
	int i=0;
	int SchedEndTime;
	while(1)
	{
		CurrentTime=NextTime;
		cout<<"\n[SIM] Current time :"<<CurrentTime<<"\n";
		for(;i<TaskList.size();)
		{
			if(TaskList[i].ArrivalTime<=CurrentTime)
			{
				if(TaskList[i].ArrivalTime==CurrentTime)
				{
					//add to ArrivedTaskList
					cout<<"[SIM] Task "<<i<<" arrived at "<<CurrentTime<<"\n\n";
					ArrivedTaskList.push_back(TaskList[i]);
				}
				i++;
			}
			else
			{
				break;
			}
		}
		if(i!=TaskList.size())
			NextTime=TaskList[i].ArrivalTime;
		else
		{
			TasksDone=1;
			NextTime=INT_MAX;
		}
		//break if no more events
		if(!TasksDone)
			SchedDone=0;
		SchedEndTime=Scheduler();
		
		//PrintATasks();
		
		NextTime=(NextTime<SchedEndTime?NextTime:SchedEndTime);
		if(TasksDone&&SchedDone)
			break;
		(*CurrentJob).RealExecutionTime -= (NextTime-CurrentTime);
		cout<<"\n[SIM] Task "<<(*CurrentJob).ID<<" executed from "<<CurrentTime
				<<" to "<<NextTime<<"\n\tRemaining exec time: "
					<<(*CurrentJob).RealExecutionTime<<"\n";
		
		if((*CurrentJob).RealExecutionTime==0)
		{
			for(int j=0;j<ArrivedTaskList.size();j++)
			{
				if(ArrivedTaskList[j].ID==(*CurrentJob).ID)
				{
					cout<<"\n\n[SIM] Task "<<(*CurrentJob).ID<<" finished execution\n";
					ArrivedTaskList.erase(ArrivedTaskList.begin()+j);
					break;
				}
			}
		}
	}
	cout<<"Simulation ended\n";
	return;
}

int main(int argc, char** argv)
{
	if(argc != 2 && argc != 3)
	{
		cout<<"Usage: \n"<<argv[0]<<" joblist [duration]\n";
	}
	ReadTasks(argv[1]);
	sort(TaskList.begin(),TaskList.end());
	PrintTasks();
	StartSim();
}