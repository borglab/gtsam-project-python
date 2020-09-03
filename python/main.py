""" Example python module. """

import argparse

import gtsam

import gtsam_example

# pylint: disable = no-member


def meaning_of_everything():
    """ return 42. """
    return 42


def create_special_2d_pose():
    """ Create a pose with (x,y) = (1,2) and theta = 3."""
    return gtsam.Pose2(1, 2, 3)


def greet():
    """
    Call the wrapped Greeting code so it can say Hello and Goodbye.
    """
    greeter = gtsam_example.Greeting()
    greeter.sayHello()
    greeter.sayGoodbye()


def parse_arguments():
    parser = argparse.ArgumentParser("Main runner for GTSAM-wrapped code")
    parser.add_argument("command",
                        help="The function to execute \
                            (e.g. 'meaning_of_everything', 'create_special_2d_pose', 'greet')")
    return parser.parse_args()


if __name__ == "__main__":
    func_map = {
        'meaning_of_everything': meaning_of_everything,
        'create_special_2d_pose': create_special_2d_pose,
        'greet': greet
    }
    args = parse_arguments()
    func = func_map.get(args.command, meaning_of_everything)
    print(func())
