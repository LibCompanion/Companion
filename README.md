![Companion](https://libcompanion.github.io/CompanionDoc/images/logo.png "Companion")

[Companion](https://libcompanion.github.io/CompanionDoc/) is a image recognition library written with OpenCV 3.

# Documentation

Documentation page can be reached at https://libcompanion.github.io/CompanionDoc/.

# Supported Plattforms

* Windows
* Universal Windows Platform (UWP)
* Linux

# Dependencies

Following dependencies are needed to build libCompanion.

* OpenCV Version 3.X
* Cuda 8 (Cuda 9 is currently not supported) to use Cuda features.

# Build Companion

Simply use cmake or cmake-gui to build Companion for your platform.
Enable the Cuda flag to build LibCompanion with implemented Cuda features.

```
cmake -DCompanion_USE_CUDA=ON/OFF
make
make install
```

# Build Companion Samples

[Samples](https://github.com/LibCompanion/CompanionSamples) are included as submodule or can be set in an cmake path.
To build samples you must enable Companion_BUILD_SAMPLES flag.

## Build Samples from Submodule

```
git submodule update --init --recursive
cmake -DCompanion_BUILD_SAMPLES=ON
make
```

## Build Samples from Project

```
git clone https://github.com/LibCompanion/CompanionSamples.git
cmake -DCompanion_BUILD_SAMPLES=ON -DCompanion_SAMPLE_MODULE=/path/to/CompanionSamples/
make
```

## UWP Support

If you desire to build Companion for *Universal Windows Platform* you can simply use the provided toolchain file to do so.
```
cmake -DCMAKE_TOOLCHAIN_FILE=UWPToolchain.cmake
```
If you prefer the cmake-gui instead, make sure to add the parameter ```CMAKE_TOOLCHAIN_FILE``` to the configuration before pressing *Configure*. Select FILEPATH as the parameter type and choose the file ```UWPToolchain.cmake``` as the parameter value.

## License

This program is an image recognition library written with OpenCV.
Copyright (C) 2016-2018 Andreas Sekulski

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
