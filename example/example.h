/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file     example.h
 * @brief    Example wrapper interface file for Python
 * @author   Varun Agrawal
 */

// This is an interface file for automatic Python wrapper generation.  See
// gtsam.h for full documentation and more examples.

#include <src/greeting.h>

namespace example {

class Greeting {
    Greeting();
    void sayHello() const;
    void sayGoodbye() const;
};

}