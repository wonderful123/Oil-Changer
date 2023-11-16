#pragma once

/**
 * @file IObserver.h
 * @brief Interface for the Observer part of the Observer pattern.
 *
 * @details
 * - This file defines the interface for Observers in the Observer pattern.
 * - Observers are notified by Subjects to which they are attached.
 * - Any component that needs to react to changes in another component should
 *   implement this interface.
 */

class IObserver {
public:
  virtual ~IObserver() = default;

  // Method called by the subject when there is an update
  virtual void update() = 0;
};
