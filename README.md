# Serial Data Decoder

This project decodes serial data received through the serial port. It accepts serial data in the following format.
        /
        A    :   5100 Kg
        B    :  17100 Kg
        C    :  22100 Kg
        D    :  15100 Kg
        TOTAL:  59400 Kg
        \
The received data is then decoded into JSON format to verify whether the TOTAL value received is correct. The 'VALID'
field indicates whether the TOTAL value is correct or not.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Limitations](#limitations)


## Introduction

The main purpose of this application is to convert serially passed data in the following format,
```code
        /
        A    :   5100 Kg
        B    :  17100 Kg
        C    :  22100 Kg
        D    :  15100 Kg
        TOTAL:  59400 Kg
        \
```
to JSON format as indicated below.

```json
    {
        "TotalWeight"   :59000,
        "VALID"         :true,
        "WeightA"       :5000,
        "WeightB"       :17000,
        "WeightC"       :22000,
        "WeightD"       :15000
    }
```

## Features

* Accept data through a given serial port.
* Capability to collect partial data received through the serial port.
* Conversion of the data to JSON format.
* Printing latest data received to standard output every 10 seconds.

## Getting Started

This application was built with g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0. It was tested on 
Linux  5.15.133.1-microsoft-standard-WSL2 on x86_64 platform.

### Prerequisites

 1. it requires 'nlohmann/json' and could be downloaded from https://github.com/nlohmann/json
 2. if unit testing can be performed of google testing framework is installed.

### Installation

 1. Clone the project to a local folder. {projdir}
 2. Clone 'nlohmann/json' package to a known location.
 3. Update INCLUDE_DIRS path of the makefile to include the directory of the nlohmann/json package.
 4. Issue make command to build the binary. 
 5. The executable binary will be created on {projdir}/build/DataPasser
 6. To execute run ./DataPasser "serial port", where "serial port" is the com port that which application 
 is required to access. Note: The application requires admin permission to access the serial port. So prefer to
 run it as sudo DataPasser /dev/ttyUSB01.
 7. To use the test build with 'google testing framework' build with the command 'make test'.


## Limitations

 1. The Tester performed only a limited number of functions.
 2. Currently support only for a maximum of 10 channels. To increase the number of channels please refer to the DataPasser::ConvertJson function.


