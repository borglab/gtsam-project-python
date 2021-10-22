#pragma once

#include <iostream>

namespace gtsam_example {

class Curse {
 public:
  /// Print some foul-mouthed slang
  void speak() const;

  void print() const { std::cout << "Some vile cursing!" << std::endl; }
};

}  // namespace gtsam_example
