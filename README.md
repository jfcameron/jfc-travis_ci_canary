[![Build Status](https://travis-ci.org/jfcameron/jfc-travis_ci_canary.svg?branch=master)](https://travis-ci.org/jfcameron/jfc-travis_ci_canary)

## travis_ci_canary

Linux system tray icon that monitors your recent travis-ci builds.

Icon graphic indicates: all builds succeeded, build list contains an ongoing build, build list contains a failure, or there is a network connection issue.

Clicking the icon will open your build list in a browser.

Notifications are emitted whenever a new build starts/stops, displaying the project name and new build state.

### Building

refer to the .travis.yml file in the root of the repository.

### How to use

The first time the program is run it will create a config file, print the path to the file and then exit. The user must write their travis-ci username and a travis-ci api token into this file for the application to run normally.
