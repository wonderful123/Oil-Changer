#include "Observer/ISubject.h"

class IDisplay : public IObserver {
public:
  virtual ~IDisplay() {}

  // Display data on the screen
  virtual void displayData(const std::string &data) = 0;

  // Update method from IObserver
  //void update(ISubject *subject) override;
    // Process the update from the subject
    // Here, you can retrieve data from the subject in a way that aligns with
    // your system's design For example, the subject might trigger different
    // types of updates, and the display would react accordingly

    // Example placeholder code (to be replaced with actual data handling
    // logic): std::string data = processData(subject); displayData(data);

  // Placeholder for a method to process data from the subject
  // virtual std::string processData(ISubject* subject) = 0;
};
