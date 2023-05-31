#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    float activeJiffies = LinuxParser::ActiveJiffies();
    float jiffies = LinuxParser::Jiffies();

    return activeJiffies/jiffies;
}