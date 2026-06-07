/// Monotonic wall-clock timer backed by CLOCK_MONOTONIC.
#pragma once

#include "ground/util/types.h"

class Timer
{
  public:
    /// Captures the current monotonic timestamp.
    void Start();

    /// Returns nanoseconds elapsed since the last Start() call.
    u64 ElapsedNs() const;

  private:
    u64 m_startNs = 0;
};
