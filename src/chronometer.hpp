#include <cassert>
#include <functional>
#include <initializer_list>
#include <ostream>
#include <string>
#include <vector>

template <typename benchTime> struct benchmarkData {
  benchTime lastTime{0};
  benchTime totalTime{0};
};

template <typename benchTime> class Chronometer {
public:
  Chronometer(const std::initializer_list<std::string> steps,
              const std::function<benchTime()> getTime)
      : numberOfFrames(0), labels(steps), times(steps.size() + 1) {
    this->getTime = getTime;
  }

  void firstStep() noexcept {
    times[0].lastTime = getTime();
    currentIndex = 1;
  }

  void nextStep() noexcept {
    times[currentIndex].lastTime = getTime();
    currentIndex++;
  }

  void finalStep() {
    times[currentIndex].lastTime = getTime();

    currentIndex++;
    assert(currentIndex == times.size());
    for (size_t i = 1; i < times.size(); i++) {
      times[i].totalTime += times[i].lastTime - times[i - 1].lastTime;
    }
    numberOfFrames++;
  }

  friend std::ostream &operator<<(std::ostream &os, Chronometer &chrono) {
    for (size_t i = 0; i < chrono.labels.size(); i++) {
      os << chrono.labels[i] << ": "
         << static_cast<float>(chrono.times[i + 1].totalTime) /
                chrono.numberOfFrames
         << " ms." << std::endl;
    }
    return os;
  }

private:
  size_t currentIndex;
  int numberOfFrames;
  std::vector<std::string> labels;
  std::vector<benchmarkData<benchTime>> times;
  std::function<benchTime()> getTime;
};
