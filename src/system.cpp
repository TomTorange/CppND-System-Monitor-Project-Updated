#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;
// std:: that you use as a prefix of many STLs. 
// but one example https://stackoverflow.com/questions/11271889/global-variable-count-ambiguous need take care the function count. 
// instead, still use: 
// using std::size_t;
// using std::
// using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return this->cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for (auto pid : pids) {
//      Process process(pid);
        processes_.emplace_back(pid);
    }
    // Sort the processes for the PID display
    std::sort(processes_.begin(),processes_.end());
    return processes_; 
}
// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }