#include "greeting.h"

namespace example {

/// Print a greeting
void Greeting::sayHello() const {
    std::cout << "Hello from GTSAM!" << std::endl;
    std::cout << "Here's a Rot3 for you " << gtsam::Rot3() << std::endl;
}

/// Print a farewell
void Greeting::sayGoodbye() const {
    std::cout << "Goodbye, robot" << std::endl;
}

};
