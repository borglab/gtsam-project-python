#include "greeting.h"

namespace gtsam_example {

/// Print a greeting
void Greeting::sayHello() const {
  std::cout << "Hello from GTSAM!" << std::endl;
  std::cout << "Here's a Rot3 for you " << gtsam::Rot3() << std::endl;
}

void Greeting::takeAPose3(const gtsam::Pose3& pose) const {
  std::cout << "Got\n" << pose << std::endl;
}

/// Process a GTSAM object
gtsam::Rot3 Greeting::invertRot3(gtsam::Rot3 rot) const {
  return rot.inverse();
}

/// Print a farewell
void Greeting::sayGoodbye() const {
  std::cout << "Goodbye, robot" << std::endl;
}

};  // namespace gtsam_example
