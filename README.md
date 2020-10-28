[![Build Status](https://travis-ci.org/jfcameron/jfc-travis_ci_canary.svg?branch=master)](https://travis-ci.org/jfcameron/jfc-travis_ci_canary) [![Coverage Status](https://coveralls.io/repos/github/jfcameron/jfc-travis_ci_canary/badge.svg?branch=master)](https://coveralls.io/github/jfcameron/jfc-travis_ci_canary?branch=master) [![Documentation](https://img.shields.io/badge/documentation-doxygen-blue.svg)](https://jfcameron.github.io/jfc-travis_ci_canary/)

## jfc-travis_ci_canary

Linux system tray icon that monitors your recent travis-ci builds.

Icon graphic indicates: all builds succeeded, build list contains an ongoing build, build list contains a failure, or there is a network connection issue.

Clicking the icon will open your build list in a browser.

Notifications are emitted whenever a new build starts/stops, displaying the project name and new build state.

### project state

application works but has no configuration or help documentation. travis ci token is provided via arg. all messages are english. currently working on config, likely ~/.config/travis-ci_canary/conf.json
