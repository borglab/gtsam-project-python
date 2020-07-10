#include "greeting.h"

namespace gtsam_example {

/// Print a greeting
void Greeting::sayHello() const {
    std::cout << "Hello from GTSAM!" << std::endl;
    std::cout << "Here's a Rot3 for you " << gtsam::Rot3() << std::endl;
}

/// Process a GTSAM object
gtsam::Rot3 Greeting::invertRot3(gtsam::Rot3 rot) const {
    return rot;
}

/// Print a farewell
void Greeting::sayGoodbye() const {
    std::cout << "Goodbye, robot" << std::endl;
}

};
