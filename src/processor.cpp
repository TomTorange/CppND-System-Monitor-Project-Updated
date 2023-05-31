#include "processor.h"
#include "linux_parser.h"

/* the processor class is responsible for the system's CPU utilization */
/* use the processor class to call the needed methods from the Linux_parser class to calculate the system CPU utilization. */
// Return the aggregate CPU utilization
float Processor::Utilization() { 
    // float activeJiffies = LinuxParser::ActiveJiffies();
    // float jiffies = LinuxParser::Jiffies();
    // // percentage of activate vs Jiffies
    // return activeJiffies/jiffies;
    return 0;
}