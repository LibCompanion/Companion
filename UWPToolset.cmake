#
# This program is an image recognition library written with OpenCV.
# Copyright (C) 2017 Dimitri Kotlovsky
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Use Universal Windows Platform as target system
set(CMAKE_SYSTEM_NAME "WindowsStore")
set(CMAKE_SYSTEM_VERSION "10.0")

# Consume Windows Runtime Extension
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW")

# Eliminate deprecation errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS")