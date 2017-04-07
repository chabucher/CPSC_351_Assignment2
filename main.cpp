// Charles Bucher
// 4/6/17
// Simulated Scheduler Program

// Significant portion of code modified/used from Professor Justin Okun of
// California State University Fullerton, Spring 2017, CPSC 351.
#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <vector>
#include <iomanip>

#include "Processor.hpp"
#include "FCFS.hpp"
#include "RR.hpp"

using namespace std;

// List of PCB structures containing statistics updated by scheduler/processor
// simulators
vector<PCB *> done_processes;


int context_switches = 0;

// Adds a completed PCB to done_processes
void AddDonePCB (PCB* pcb)
{
    done_processes.push_back(pcb);
}

// Removes all PCBs from done_processes
void ClearDonePCBs()
{
    done_processes.clear();
}

// Calculates the turnaround time for a done process
float ProcessTurnaroundTime(PCB* pcb)
{
    float turnaround = 0;
    turnaround = pcb->_waitTicks + pcb->_workTicks;
    
    return turnaround;
}

// Returns the average turnaround time for all done processes
float AverageTurnaroundTime()
{
    float average = 0;
    int items = (int)done_processes.size();
    for(int i = 0; i < items; i++)
    {
        average += ProcessTurnaroundTime(done_processes[i]);
    }
    average = average / items;
    
    return average;
}

// Returns the average response time for all done processes
float AverageResponseTime ()
{
    float average = 0;
    int items = (int)done_processes.size();
    for(int i = 0; i < items; i++)
    {
        average += done_processes[i]->_responseTicks;
    }
    average = average / items;
    
    return average;
}

// Returns the average wait time for all done processes
float AverageWaitTime()
{
    float average = 0;
    int items = (int)done_processes.size();
    for(int i = 0; i < items; i++)
    {
        average += done_processes[i]->_waitTicks;
    }
    average = average / items;
    
    return average;
}

// Returns the number of context switches that occured during the trial
int ContextSwitches()
{
    return context_switches;
}

// Returns the total number of ticks that were required to run all of the
// processes to completion.
float TotalWorkTime()
{
    float total = 0;
    int items = (int)done_processes.size();
    for(int i = 0; i < items; i++)
    {
        total += done_processes[i]->_workTicks;
    }
    
    return total;
}

// Prints the end statistics for each simulation trial
void PrintStatistcs()
{
    cout << "Total Work Time: " << TotalWorkTime() << endl
        << "Avg. Turnaround Time: " << AverageTurnaroundTime() << endl
        << "Avg. Wait Time: " << AverageWaitTime() << endl
        << "Avg. Response Time: " << AverageResponseTime() << endl
        << "Context Switches: " << ContextSwitches() << endl
        << endl;
}

// Adds PCBs with short cpu bursts to the simulation
void AddShortBurstProcesses(Scheduler* sched)
{
    sched->Enqueue(new PCB (8));
    sched->Enqueue(new PCB (3));
    sched->Enqueue(new PCB (5));
    sched->Enqueue(new PCB (4));
    sched->Enqueue(new PCB (1));
    sched->Enqueue(new PCB (2));
    sched->Enqueue(new PCB (4));
    sched->Enqueue(new PCB (6));
    sched->Enqueue(new PCB (3));
    sched->Enqueue(new PCB (7));
}

// Adds PCBs with long cpu bursts to the simulation
void AddLongBurstProcesses(Scheduler* sched)
{
    sched->Enqueue(new PCB (14));
    sched->Enqueue(new PCB (24));
    sched->Enqueue(new PCB (32));
    sched->Enqueue(new PCB (64));
    sched->Enqueue(new PCB (80));
    sched->Enqueue(new PCB (26));
    sched->Enqueue(new PCB (45));
    sched->Enqueue(new PCB (36));
    sched->Enqueue(new PCB (23));
    sched->Enqueue(new PCB (28));
}

// A couple of test PCBs - used to compare against simple hand calculations
void AddTestProcesses (Scheduler* sched)
{
    sched->Enqueue(new PCB (24));
    sched->Enqueue(new PCB (3));
    sched->Enqueue(new PCB (3));
}

// Performs a context switch in the simulated environment
void ContextSwitch(Scheduler* sched, Processor & cpu)
{
    PCB* oldPCB = cpu.DeactivateProcess ();
    
    if (oldPCB != NULL)
    {
        if (false == oldPCB->IsProcessDone())
            sched->Enqueue(oldPCB);
        else
            AddDonePCB(oldPCB);
    }
    
    PCB* newPCB = sched->Dequeue();
    
    if (newPCB != NULL)
    {
        context_switches++;
        cpu.ActivateProcess(newPCB);
    }
}

// Executes the simulation
void RunSimulation (Scheduler* sched)
{
    // The simulated processor
    Processor cpu;
    
    bool go = true;
    context_switches = 0;
    
    // go is true as long as there are processes with work to do.
    while (go)
    {
        // Need to context switch when either the processor is idle or the
        // scheduler decides its necessary
        if (cpu.IsIdle() || sched->PreemptProcess())
        {
            ContextSwitch(sched, cpu);
        }
        
        // Tick the clock for the environment
        cpu.ProcessTick();
        sched->ProcessTick();
        
        // Assume the simulation is over when the scheduler's ready queue is
        // empty and cpu is idle
        if (sched->PCBCount() <= 0 && cpu.IsIdle())
        {
            AddDonePCB(cpu.DeactivateProcess ());
            go = false;
        }
    }
}

void fcfs_test()
{
    ClearDonePCBs();
    
    cout << "FCFS Test Run" << endl;
    
    FCFS fcfs_scheduler;
    
    AddTestProcesses(&fcfs_scheduler);
    
    RunSimulation(&fcfs_scheduler);
    
    PrintStatistcs();
}

void rr_test()
{
    ClearDonePCBs();
    
    cout << "RR Test Run: quantum = 4" << endl;
    
    RR rr_scheduler(4);

    AddTestProcesses(&rr_scheduler);
    
    RunSimulation(&rr_scheduler);
    
    PrintStatistcs();
}

void fcfs_trial1 ()
{
    ClearDonePCBs();
    
    cout << "FCFS Trial 1: short burst processes"  << endl;
    
    FCFS fcfs_scheduler;
    
    AddShortBurstProcesses(&fcfs_scheduler);
    
    RunSimulation(&fcfs_scheduler);
    
    PrintStatistcs();
}

void fcfs_trial2 ()
{
    ClearDonePCBs();
    
    cout << "FCFS Trial 2: long burst processes" << endl;
    
    FCFS fcfs_scheduler;
    
    AddLongBurstProcesses(&fcfs_scheduler);
    
    RunSimulation(&fcfs_scheduler);
    
    PrintStatistcs();
}

void fcfs_trial3 ()
{
    ClearDonePCBs();
    
    cout << "FCFS Trial 3: short burst processes followed by "
          << "long burst processes" << endl;
    
    FCFS fcfs_scheduler;
    
    AddShortBurstProcesses(&fcfs_scheduler);
    AddLongBurstProcesses(&fcfs_scheduler);
    
    RunSimulation(&fcfs_scheduler);
    
    PrintStatistcs();
}

void fcfs_trial4 ()
{
    ClearDonePCBs();
    
    cout << "FCFS Trial 4: long burst processes followed by "
          << "short burst processes" << endl;
    
    FCFS fcfs_scheduler;
    
    AddLongBurstProcesses(&fcfs_scheduler);
    AddShortBurstProcesses(&fcfs_scheduler);
    
    RunSimulation(&fcfs_scheduler);
    
    PrintStatistcs();
}

void rr_trial1()
{
    ClearDonePCBs();
    
    cout << "RR Trial 1: quantum = 10, short burst processes" << endl;
    
    RR rr_scheduler(10);
    
    AddShortBurstProcesses(&rr_scheduler);
    
    RunSimulation(&rr_scheduler);
    
    PrintStatistcs();
}

void rr_trial2()
{
    ClearDonePCBs();
    
    cout << "RR Trial 2: quantum = 10, long burst processes" << endl;
    
    RR rr_scheduler(10);
    
    AddLongBurstProcesses(&rr_scheduler);
    
    RunSimulation(&rr_scheduler);
    
    PrintStatistcs();
}

void rr_trial3()
{
    ClearDonePCBs();
    
    cout << "RR Trial 3: quantum = 4, short burst processes" << endl;
    
    RR rr_scheduler(4);
    
    AddShortBurstProcesses(&rr_scheduler);
    
    RunSimulation(&rr_scheduler);
    
    PrintStatistcs();
}

void rr_trial4()
{
    ClearDonePCBs();
    
    cout << "RR Trial 4: quantum = 4, short burst processes followed by "
          << "long burst processes" << endl;
    
    RR rr_scheduler(4);
    
    AddShortBurstProcesses(&rr_scheduler);
    AddLongBurstProcesses(&rr_scheduler);
    
    RunSimulation(&rr_scheduler);
    
    PrintStatistcs();
}

void rr_trial5()
{
    ClearDonePCBs();
    
    cout << "RR Trial 5: quantum = 4, long burst processes followed by "
          << "short burst processes" << endl;
    
    RR rr_scheduler(4);
    
    
    AddLongBurstProcesses(&rr_scheduler);
    AddShortBurstProcesses(&rr_scheduler);
    
    RunSimulation(&rr_scheduler);
    
    PrintStatistcs();
}


int main()
{
    fcfs_test();   // Small set of processes. Compare against hand calculations
    rr_test();     // Small set of processes. Compare against hand calculations
    fcfs_trial1(); // FCFS: short burst processes
    fcfs_trial2(); // FCFS: long burst processes
    fcfs_trial3(); // FCFS: short burst processes followed by
                   //   long burst processes
    fcfs_trial4(); // FCFS: long burst processes followed by
                   //   short burst processes
    rr_trial1();   // RR: quantum = 10, short burst processes
    rr_trial2();   // RR: quantum = 10, long burst processes
    rr_trial3();   // RR: quantum = 4 , short burst processes
    rr_trial4();   // RR: quantum = 4 short burst processes followed by
                   //   long burst processes
    rr_trial5();   // RR: quantum = 4, long burst processes followed by
                   //   short burst processes
     
    cout << "Press enter to exit." << endl;
    cin.get();
    
    return 0;
}
