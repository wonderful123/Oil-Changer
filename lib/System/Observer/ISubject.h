#pragma once

#include "IObserver.h"

/**
 * @file ISubject.h
 * @brief Interface for the Subject part of the Observer pattern.
 *
 * @details
 * - This file defines the interface for Subjects in the Observer pattern.
 * - Subjects notify observers about significant occurrences or state changes.
 * - Any component that needs to be observed should implement this interface.
 */

class ISubject {
public:
  virtual ~ISubject() = default;

  // Attach an observer to the subject
  virtual void attach(IObserver *observer) = 0;

  // Detach an observer from the subject
  virtual void detach(IObserver *observer) = 0;

  // Notify all attached observers about a change
  virtual void notify(EventType eventType) = 0;
};
