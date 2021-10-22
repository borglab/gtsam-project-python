/* ----------------------------------------------------------------------------
 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)
 *
 * See LICENSE for the license information
 * -------------------------------------------------------------------------- */

/**
 * @file     gtsam_example.h
 * @brief    Example wrapper interface file for Python
 * @author   Varun Agrawal
 */

// This is an interface file for automatic Python wrapper generation.
// See gtsam.h for full documentation and more examples.

#include <cpp/greeting.h>

// The namespace should be the same as in the c++ source code.
namespace gtsam_example {

class Greeting {
  Greeting();
  gtsam::noiseModel::Base* model;

  void sayHello() const;
  void takeAPose3(const gtsam::Pose3& pose) const;
  gtsam::Rot3 invertRot3(gtsam::Rot3 rot) const;
  void sayGoodbye() const;
  Matrix getMatrix(gtsam::noiseModel::Base* model) const;

  string operator()(const string& name) const;

  void insertName(const string& name);
  string operator[](size_t idx) const;

  void print() const;
};

}  // namespace gtsam_example
