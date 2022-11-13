# Payload-Sims

Repository for GEANT4 simulations created for the Payload 2020/2021 Project

## PREREQUISITES

Before building this project, you need to
[install the Geant4 Toolkit](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/gettingstarted.html).

TLDR, download and extract
[the source code](https://geant4.web.cern.ch/support/download).
Then create a source directory
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
Building geant4 might take a while (sit tight).

Next we need to export a bunch of environment variables, Geant4 provides a
handy script for this (you should add this to your `.bashrc` or equivalent):
```
$ source <path-to-geant4-dir>/geant4-v11.0.2-install/bin/geant4.sh
```

## BUILDING

We are now ready to build `Payload-Sims`, run the following inside the root of
the project:
```
$ mkdir build
$ cd build
$ cmake ../
$ make -j$(nproc)
```

