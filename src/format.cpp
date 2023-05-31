#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    // calculate the number of hours first
    int hour = seconds/3600;
    // after getting the number of hours, we should subtract it from the total number of seconds
    int seconds_over_hour = seconds - hour*3600;
    // Calculate minutes
    int minute = seconds_over_hour/60;
    // Calculate seconds over minutes (rest)
    int second = seconds_over_hour%60;
    
    return std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
}