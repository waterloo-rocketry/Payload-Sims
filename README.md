# Payload-Sims

Repository for GEANT4 simulations created for the Payload 2020/2021/22 Project. The aim is to simulate the behaviour of the 2022 Payload at upper atmosphere by shooting gamma rays and neutrons at the payload model placed in thin atmosphere.


Note that this guide is written for Linux. There are extra steps involved in the installation guide of Windows and additional software pre-reqs, which will be detailed in the Additional Notes.

## Prerequisites

Software Prerequisites for running the Simulation on Linux includes, but may not be limited to
- GCC (Check with Geant4 website for the version)
- CMake
- Geant4 Toolkit source code

Additionally, OpenGL (X11) and Qt is recommended as a visualization option as pre-reqs.

For Windows, see additional notes for details regarding pre-reqs and build details.

## Installation
Before running this project, you need to install the Geant4 Toolkit. Setup instructions can be found [https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/gettingstarted.html](here). 

TLDR, download and extract source code from
[https://geant4.web.cern.ch/support/download](here). Create a source directory
like this:
```
geant4
├── geant4-v11.0.2
├── geant4-v11.0.2-build
└── geant4-v11.0.2-install
```

Now run the following
```
$ cd geant4-v11.0.2-build
$ cmake -DCMAKE_INSTALL_PREFIX=<path-to-geant4-dir>/geant4-v11.0.2-install \
        -DGEANT4_INSTALL_DATA=ON \
        -DGEANT4_USE_OPENGL_X11=ON \
        -DGEANT4_USE_QT=ON \
        ../geant4-v11.0.2
$ make -j <number-of-cores>
$ make install
```
Compiling and building Geant4 might take a while (sit tight).

Next we need to export a bunch of environment variables, Geant4 provides a
handy script for this (you should add this to your `.bashrc` or equivalent):
```
$ source <path-to-geant4-dir>/geant4-v11.0.2-install/bin/geant4.sh
```

## Build

The building process of the Payload-Sims project is very similar to the process of building the exampleB1. Simply put, follow this guide, but swap out B1-build for the name of your cloned repository (e.g. Sims-build). [https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/GettingStarted/makeFile.html] (here)

We are now ready to build `Payload-Sims`, run the following inside the root of
the project:
```
$ mkdir build
$ cd build
$ cmake ../
$ make -j <number-of-cores>
```

Note that changing the setup of the experiment will require rebuilding and remaking of the Payload Simulation. See the documentation in src for details about the experiment setup.
## Running

After the build, an executable by the name of BuildTest should be available to be ran. The executable provides an interface in which you can run tests by generating particles and the cumulative dosage of the counting volume will be tallied at the end of each action. You can also use it to generate visualizations.

## Useful macros and commands
Upon entering the interface of the executable, there are a couple of useful commands to run.

```
help -- jumps to documentation of various commands. Run it in BuildTest.
/run/initialize -- initializes the geometry and environment. This needs to be done before any tests.
/run/beamOn n -- runs n beams of particles in the environment, each time it is ran, a summary of dosage will be generated.
/vis/viewer/update -- update the display and generate a new visualization file where applicable.
```

The commands can also be fed into the executable on command line from a macro file. To do so, run the executable as below.
```
$ ./BuildTest < macrofile.mac
```

Sample macro files can be found under the macros folder. Note that init.vis may be ran by default.

## Visualization

If installed with OpenGL and Qt during setup as flags (as shown in prior sections), init.mac can be used to generate an OpenGL + Qt viewer along with approproiate visual settings. Otherwise, the testGPS.mac will provide a fully text-based interface with the results of each event displayed in console. 
testGPS.mac also provides option to output HepRApp visualization files should the visualization commands commented out with `#` be used. A heprep file will be generated at the end of each event. These visualization files can be opened by Heprep.jar, by running
```
$ java -jar heprep.jar
```
which will allow you to load visualization files via the menus inside the HepRApp visualization engine.

## Additional Notes
Additional documentation is provided under src for a brief walkthough of the source code. Aditionally, comments are added in the macro files to briefly explain each command.

### Setup on Windows

#### Prerequisites
Instead of GCC, running Geant4 on Windows requires MSVC alongside Microsoft Visual Studio 2019 at time of writing. The Desktop Development option (with the optionals selected) should be sufficient. Additionally, you may pick out only the necessary MSVC compiler and CMake tools, though we have not tested the minimum toolset needed. 

#### Build and Installation
The commands and executables generated from building Geant4 and the Simulation will be sligtly different from the ones generated on Linux. You will need to check out the `Windows Platform` Sections of the same guides as above for detailed installation and running. The behaviour of the executable should be the same across platforms. It is important to note that most of the build and make commands for installing Geant4 Toolkit and the Simulation itself need to be ran on the Developer Command Prompt of MSVS 2019.

#### Visualization
HepRApp should be the same across platform; however, OPENGL_X11 will not be available on Windows. Thus, should OpenGL/Qt be available on Windows, the GEANT4_USE_OPENGL_WIN32 flag should be used instead.
