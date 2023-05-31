#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {  
  float totMem, freeMem;
  string line;
  string lable;
  float value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> lable >> value;
      if (lable == "MemTotal") {
        totMem = value;
      }
      if (lable == "MemFree") {
        freeMem =value;
      }
    }
  }
  return ((totMem - freeMem)/totMem);
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long systemTime;
  string line;
  string upTimeValue;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTimeValue;
    systemTime = std::stol(upTimeValue);
  }
  return systemTime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string ignorekey;
  long value{0};
  long activeJiffies{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; ++i) {
      linestream >> ignorekey;
    }
    for (int i = 0; i < 4; ++i) {
      linestream >> value;
      activeJiffies += value;
    }
    return activeJiffies;
  }
  return activeJiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto cpuUtil = CpuUtilization();
  return stol(cpuUtil[CPUStates::kUser_]) + stol(cpuUtil[CPUStates::kNice_]) + stol(cpuUtil[CPUStates::kSystem_]) +
         stol(cpuUtil[CPUStates::kIRQ_]) + stol(cpuUtil[CPUStates::kSoftIRQ_]) + stol(cpuUtil[CPUStates::kSteal_]);
 }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto cpuUtil = CpuUtilization();
  return stol(cpuUtil[CPUStates::kIdle_]) + stol(cpuUtil[CPUStates::kIOwait_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuUtil;
  string line, seg;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    for (unsigned int i = 0; i <= 10; i++){
      linestream >> seg;
      if ( seg != "cpu" ){
        cpuUtil.push_back(seg);
      }
    }
  }
  return cpuUtil;
  }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string processName;
  int processValue;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> processName >> processValue;
      if (processName == "processes") break;
    }
 }
 return processValue;
}
// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string runningProcess;
  int runningProcessValue;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> runningProcess >> runningProcessValue;
      if (runningProcess == "procs_running") break;
    }
  }
  return runningProcessValue;
 }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string command;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string Ram;
  string RamValue;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> Ram >> RamValue;
      if (Ram == "VmSize") {
        break;
      }
    }
  }
  return (std::to_string(stol(RamValue)/1000));
 }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  string line;
  string Uid;
  string UidValue;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ';', ' ');
      std::istringstream linestream(line);
      linestream >> Uid >> UidValue;
      if (Uid == "Uid") {
        break;
      }
    }
  }
  return UidValue;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string User;
  string password, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> User >> password >> uid;
      if (uid == LinuxParser::Uid(pid)){
        break;
      }
    }
  }
  return User;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string upTime = "0";
 // vector<string> upTimes;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++){
      linestream >> upTime;
    }
  }
  return std::stol(upTime)/sysconf(_SC_CLK_TCK);
}
