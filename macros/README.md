# Macros
## Usage:
The two files in the directory are macro commands meant to be ran in BuildTest. To feed it in, simply pipe the file in like this
```
$ BuildTest < init_vis.mac > output.txt
```
The outputs of the run can also be redirected to be written to a file as above.

Specifically, the init_vis macro sets up the Qt graphics settings, which is optional.
The testGPS macro sets up the actual test environment and runs the tests. The visualization options commented out in testGPS is reserved for HepRApp file generation.

## Dissection of macro files and manual commands:
Detailed discussions of each macro command can be found by running `help` inside the Executable.
The main layout of the command sequence is as follows
- Initialization of environment and geometry overlap check: /run/initialize
- Visualization settings: /vis/...
- Particle Generation Settings for initial particle types and values: /gps/...
- Generate beam and track (n particles), results will be generated onto the output: /run/beamOn n
- Exit: exit
