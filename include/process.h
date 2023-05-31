#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int processId);
  int Pid();                               
  std::string User();                     
  std::string Command();                   
  float CpuUtilization();                 
  std::string Ram();                      
  long int UpTime();                       
  bool operator<(Process const& a) const; 

  // Declare any necessary private members
 private:
 int Pid_;
 std::string user_;
 std::string command_;
 float cpuUtilization_;
 std::string ram_;
 long int upTime_;
};

#endif