#pragma once

#include <chrono>
#include <iostream>

namespace timer {

using namespace std::chrono;

class CountdownTimer {
 public:
  CountdownTimer();
  CountdownTimer(int milliseconds);

  void setDuration(int count);
  void start();
  void restart();
  bool isRunning();
  bool isExpired() const;
  long long remainingSeconds() const;

 private:
  steady_clock::time_point startTime_;
  milliseconds duration_;
  bool isRunning_;
};

}  // namespace timer
