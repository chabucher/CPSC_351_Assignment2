#include "RR.hpp"
#include <iostream>
using namespace std;

RR::QuantumTimer::QuantumTimer (int quantum) :_quantum(quantum)
{
	Reset ();
}

void RR::QuantumTimer::Reset ()
{
	_timer = _quantum;
}
	
bool RR::QuantumTimer::IsExpired ()
{
	if (_timer <= 0)
		return true;
			
	return false;
}
	
void RR::QuantumTimer::ProcessTick ()
{
	_timer--;
}

RR::RR() :FCFS(), _timer(4)
{
}

RR::RR(int quantum) :FCFS(), _timer(quantum)
{
}

PCB* RR::Dequeue ()
{
    this->_timer.Reset();
    
    PCB * ret_val = this->readyQueue.back();
    this->readyQueue.pop_back();
    
	return ret_val;
}
	
void RR::ProcessTick()
{
    FCFS::ProcessTick();
    this->_timer.ProcessTick();
}

bool RR::IsPremptive()
{
    return true; // A RR is preemptive by definition
}
	
bool RR::PreemptProcess()
{
    return IsPremptive() && this->_timer.IsExpired();
}
