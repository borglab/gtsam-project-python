#include <gtsam/geometry/Pose3.h>
#include <gtsam/linear/NoiseModel.h>

#include <iostream>
#include <string>

namespace gtsam_example {

class Greeting {
 public:
  gtsam::noiseModel::Base::shared_ptr model =
      gtsam::noiseModel::Isotropic::Sigma(1, 0.1);

  /// Print a greeting
  void sayHello() const;

  void takeAPose3(const gtsam::Pose3& pose) const;

  /// Process a GTSAM object
  gtsam::Rot3 invertRot3(gtsam::Rot3 rot) const;

  /// Print a farewell
  void sayGoodbye() const;

  gtsam::Matrix getMatrix(gtsam::noiseModel::Base::shared_ptr model) const {
    return gtsam::Matrix3::Zero();
  }
};

}  // namespace gtsam_example
