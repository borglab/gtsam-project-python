""" Example python module. """

import gtsam

# pylint: disable = no-member


def meaning_of_everything():
    """ return 42. """
    return 42


def create_special_2d_pose():
    """ Create a pose with (x,y) = (1,2) and theta = 3."""
    return gtsam.Pose2(1, 2, 3)
