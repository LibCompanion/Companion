![Companion](https://libcompanion.github.io/libCompanion/images/logo.png "Companion")

[Companion](https://libcompanion.github.io/libCompanion/) is an computer vision library written with OpenCV 3.

# Dependencies

* OpenCV Version 3.X
* OpenMP
* Cuda*

\* Optional

# CMake Project

Stable build from Companion is under libCompanion folder. Main folder contains
example configuration setups how to use Companion and testing folder contains
unstable algorithms which are planned features to implement in Companion.

## Build Companion

Simply use cmake or cmake-gui to build Companion for you platform. Enable the Cuda flag to build Companion with Cuda support.

```
cmake -DUSE_CUDA=ON
make
```

# Getting started

An usage example for an 2D Object detection setup is implemented under Main folder. For more Information go to this [getting started](https://nepitwin.github.io/libCompanion/gettingstarted/) Guide.

### License

This program is an image recognition library written with OpenCV.
Copyright (C) 2016-2017 Andreas Sekulski

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
