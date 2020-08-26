"""
GTSAM Copyright 2010-2019, Georgia Tech Research Corporation,
Atlanta, Georgia 30332-0415
All Rights Reserved

See LICENSE for the license information

Cal3Unified unit tests.
Author: Frank Dellaert & Duy Nguyen Ta (Python)
"""
import unittest

import numpy as np

import gtsam
import gtsam_example
from gtsam.utils.test_case import GtsamTestCase


class TestExample(GtsamTestCase):

    def test_Hello(self):
        g = gtsam_example.Greeting()
        g.sayHello()

    def test_invertRot3(self):
        g = gtsam_example.Greeting()
        r = gtsam.Rot3()
        g.invertRot3(r)


if __name__ == "__main__":
    unittest.main()
