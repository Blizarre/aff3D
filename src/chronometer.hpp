#include <cassert>
#include <functional>
#include <ostream>
#include <string>

template <typename chronoTime> class Chronometer {
public:
  Chronometer(const std::string &label,
              const std::function<chronoTime()> getTime)
      : numberOfCalls(0), label(label), totalTime(0) {
    this->getTime = getTime;
  }

  void addTimeSince(chronoTime startTime) {
    endTime = getTime();
    totalTime += endTime - startTime;
    numberOfCalls++;
  }

  chronoTime lastEndTime() { return endTime; }

  friend std::ostream &operator<<(std::ostream &os, Chronometer &chrono) {
    os << chrono.label << ": "
       << static_cast<float>(chrono.totalTime) / chrono.numberOfCalls << " ms."
       << std::endl;
    return os;
  }

private:
  unsigned int numberOfCalls;
  chronoTime endTime;
  std::string label;
  chronoTime totalTime;
  std::function<chronoTime()> getTime;
};
