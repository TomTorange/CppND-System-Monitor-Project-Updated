#include "processor.h"
#include "linux_parser.h"

/* the processor class is responsible for the system's CPU utilization */
/* use the processor class to call the needed methods from the Linux_parser class to calculate the system CPU utilization. */
// Return the aggregate CPU utilization
float Processor::Utilization() {
     float CPU{0};
     if (LinuxParser::Jiffies() == 0)
       throw std::invalid_argument("CPU util is zero!");
     CPU = float(LinuxParser::ActiveJiffies())/float(LinuxParser::Jiffies());
 return CPU;
}