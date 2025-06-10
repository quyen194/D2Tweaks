
#include "common/timer.h"

namespace timer {

CountdownTimer::CountdownTimer() : duration_(0), isRunning_(false) {}

CountdownTimer::CountdownTimer(int milliseconds)
    : duration_(0), isRunning_(false) {
  setDuration(milliseconds);
  start();
}

void CountdownTimer::setDuration(int count) {
  duration_ = milliseconds(count);
  isRunning_ = false;
}

void CountdownTimer::start() {
  startTime_ = steady_clock::now();
  isRunning_ = true;
}

void CountdownTimer::restart() { start(); }

bool CountdownTimer::isRunning() { return isRunning_; }

bool CountdownTimer::isExpired() const {
  if (!isRunning_)
    return false;

  auto now = steady_clock::now();
  return now - startTime_ >= duration_;
}

long long CountdownTimer::remainingSeconds() const {
  if (!isRunning_)
    return duration_cast<seconds>(duration_).count();

  auto now = steady_clock::now();
  auto remaining = duration_ - (now - startTime_);
  return duration_cast<seconds>(remaining).count();
}

}  // namespace timer
