#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <iostream>


using std::string;
using std::to_string;
using std::vector;


// The process class is the one which is responsible for the processes running on the system. 
// In other words, you should use it to call the methods you have written in the Linux_parser 
// which deals with processes like Ram, common, user.. etc.

// Return this process's ID
Process::Process(int processId) { Pid_ = processId; }
int Process::Pid() { return this->Pid_; }

// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
// #1 uptime of the system (seconds)
// #14 utime - CPU time spent in user code, measured in clock ticks
// #15 stime - CPU time spent in kernel code, measured in clock ticks
// #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
// #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
// #22 starttime - Time when the process started, measured in clock ticks
// total_time = utime + stime + cutime + cstime
// seconds = uptime - (starttime / Hertz)
// cpu_usage = 100 * ((total_time / Hertz) / seconds)
// divide the "clock ticks" value by sysconf(_SC_CLK_TCK) to convert to seconds
// reference: https://knowledge.udacity.com/questions/866606
float Process::CpuUtilization() {
  string path = LinuxParser::kProcDirectory + to_string(Pid_) + LinuxParser::kStatFilename;
  std::ifstream stream(path);
  string line;
  std::getline(stream, line);
  std::istringstream buffer(line);
  std::istream_iterator<string> begin(buffer), end;
  vector<string> line_content(begin, end);
  float uptime = LinuxParser::UpTime();
  float utime = stof(line_content[13]);
  float stime = stof(line_content[14]);
  float cutime = stof(line_content[15]);
  float cstime = stof(line_content[16]);
  float starttime = stof(line_content[21]);
  float heartz = sysconf(_SC_CLK_TCK);
  float totaltime = utime + stime + cutime + cstime;
  float seconds = uptime - (starttime / heartz);
  if (seconds > 0) return ((totaltime / heartz) / seconds);
  return 0;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
// UpTime(): system up time
// UpTime(Pid()) : the time the process started after system boot
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return  this->cpuUtilization_ < a.cpuUtilization_; }