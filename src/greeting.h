#include <gtsam/geometry/Rot3.h>
#include <iostream>
#include <string>

namespace gtsam_example {

class Greeting {
 public:
  /// Print a greeting
  void sayHello() const;

  /// Process a GTSAM object
  gtsam::Rot3 invertRot3(gtsam::Rot3 rot) const;
  
  /// Print a farewell
  void sayGoodbye() const;
};

}  // namespace gtsam_example
