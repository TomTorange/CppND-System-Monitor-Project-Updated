#include <string>
#include <iomanip>
#include "format.h"
#include <chrono>

using std::string;
using std::setfill;
using std::setw;
using namespace std::chrono;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// https://knowledge.udacity.com/questions/983591
// https://knowledge.udacity.com/questions/155686
// https://review.udacity.com/#!/reviews/4026529
string Format::ElapsedTime(long s) {
  std::chrono::seconds seconds{s};

  std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

  std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

  std::stringstream ss{};

  ss << std::setw(2) << std::setfill('0') << hours.count()     // HH
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << minutes.count()   // MM
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << seconds.count();  // SS

  return ss.str();
}