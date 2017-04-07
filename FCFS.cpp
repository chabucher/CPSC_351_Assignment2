#include "FCFS.hpp"

FCFS::FCFS () :Scheduler()
{
}
		
// Enqueues a process in the ready queue
void FCFS::Enqueue (PCB* pcb)
{
    readyQueue.push_front(pcb);
}
		
// Dequeues a process from the ready queue to run on the processor
PCB* FCFS::Dequeue ()
{
    if(readyQueue.size() <= 0)
    {
        return NULL;
    }
    else
    {
        PCB * ret_val = readyQueue.back();
        readyQueue.pop_back();
        return ret_val;
    }
}

// Processes a tick, which updates PCB waiting statistics  
void FCFS::ProcessTick()
{
    std::list<PCB *>::iterator it;
    for(it = readyQueue.begin(); it != readyQueue.end(); ++it)
    {
	 (*it)->_waitTicks++;
    }
}
		
// Returns the number of PCBs in the ready queue
int FCFS::PCBCount()
{
	return (int)readyQueue.size();
}

// Returns true if the scheduler is preemptive
bool FCFS::IsPremptive()
{
	return false; // A FCFS is non-preemptive by definition
}
		
// Returns true when it is time to interrupt the running process
bool FCFS::PreemptProcess()
{
    return IsPremptive();
}
