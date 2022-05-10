# Overview
The [MQTT-SN](http://mqtt.org/2013/12/mqtt-for-sensor-networks-mqtt-sn) 
protocol is a sibling of [MQTT](http://mqtt.org). While **MQTT** is designed to be 
used over a **reliable stream** transport protocol (such as TCP/IP), the **MQTT-SN**
was designed to be used over a **datagram** transport protocol.

The protocol is defined in the [schema](dsl/schema.xml)
file using [CommsDSL](https://github.com/commschamp/CommsDSL-Specification).
The **commsds2comms** code generator from [commsdsl](https://github.com/commschamp/commsdsl)
project is used to generate C++11 code of the protocol implementation.

The [src](src) folder contains additional and/or overriding default functionality 
code snippets, that get injected into the generated code.

The **generated** CMake project of this protocol definition is hosted and can be
viewed at [cc.mqttsn.generated](https://github.com/commschamp/cc.mqttsn.generated)

# License
Please read [License](https://github.com/commschamp/commsdsl#license)
section from [commsdsl](https://github.com/commschamp/commsdsl) project.

# How to Build
This project uses CMake as its build system. Please open main
[CMakeLists.txt](CMakeLists.txt) file and review available options as well as
mentioned available parameters, which can be used in addition to standard 
ones provided by CMake itself, to modify the default build. 

The **commsdsl2comms** application from [commsdsl](https://github.com/commschamp/commsdsl)
project is used to generate appropriate C++ code. If path to externally
built **commsdsl2comms** is not provided, then this project will build the
latter itself.

If code generation output directory is not provided, it defaults to 
`output` subdirectory of the one used to build the project.

### Linux Build
```
$> cd /source/of/this/project
$> mkdir build && cd build
$> cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install ..
$> make 
```

### Windows Build
```
$> cd C:\source\of\this\project
$> mkdir build && cd build
$> cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release \ 
    -DCMAKE_INSTALL_PREFIX=%cd%/install 
$> nmake 
```

# Supported Compilers
Please read [Supported Compilers](https://github.com/commschamp/commsdsl#supported-compilers)
info from [commsdsl](https://github.com/commschamp/commsdsl) project.

# How to Build and Use Generated Code
Please read the
[Generated CMake Project Walkthrough](https://github.com/commschamp/commsdsl/blob/master/doc/GeneratedProjectWalkthrough.md)
documentation page for details on the generated project internals.

The [release](https://github.com/commschamp/cc.mqttsn.commsdsl/releases)
artifacts contain doxygen generated documentation of the protocol definition.

# Contact Information
For bug reports, feature requests, or any other question you may open an issue
here in **github** or e-mail me directly to: **arobenko@gmail.com**. I usually
respond within 24 hours.

