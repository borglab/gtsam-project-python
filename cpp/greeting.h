#pragma once

#include <gtsam/geometry/Pose3.h>
#include <gtsam/linear/NoiseModel.h>

#include <iostream>
#include <string>
#include <vector>

namespace gtsam_example {

class Greeting {
  std::vector<std::string> names_;

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

  std::string operator()(const std::string& name) const {
    return "Function invoked by " + name;
  }

  void insertName(const std::string& name) { names_.push_back(name); }

  std::string operator[](size_t idx) const { return this->names_[idx]; }

  void print() const {
    std::cout << "This is a greeting" << std::endl;
  }
};

}  // namespace gtsam_example
