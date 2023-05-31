#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return this->Pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return this->cpuUtilization_; }

// Return the command that generated this process
string Process::Command() { return this->command_; }

// Return this process's memory utilization
string Process::Ram() { return this->ram_; }

// Return the user (name) that generated this process
string Process::User() { return this->user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return this->upTime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return  this->cpuUtilization_ < a.cpuUtilization_; }