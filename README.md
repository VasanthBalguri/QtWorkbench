# QtWorkbench

Created for qt learning purpose

prequesites:
install opencascade, openscenegraph and qt

sudo apt-get install build-essential oce openscenegraph qt5-default boost

build steps:

* create bin directory
* go to bin directory
* run cmake ..
* run make
* Qtworkbench executable will be generated in build folder

Create appimage:
* run appbuild.sh


# Current usage

* Added sources from Opencascade tutorial to create bottle(should be removed)
* There is a feature to open .step files

# Screenshots
![Alt text](/screenshots/sample1.png?raw=true "Opencascade Bottle")
![Alt text](/screenshots/sample2.png?raw=true "osg file")
# note:

* Need to test CMake build on windows to confirm cross compatibility
