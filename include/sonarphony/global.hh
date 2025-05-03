// This file is part of SonarPhony.
//
// SonarPhony is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SonarPhony is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SonarPhony.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef SONARPHONY_LIBRARY
    #ifdef __GNUC__
      #define SONARPHONY_EXPORT __attribute__ ((dllexport))
    #else
      #define SONARPHONY_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define SONARPHONY_EXPORT __attribute__ ((dllimport))
    #else
      #define SONARPHONY_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define SONARPHONY_LOCAL
#else
  #if __GNUC__ >= 4
    #define SONARPHONY_EXPORT __attribute__ ((visibility ("default")))
    #define SONARPHONY_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define SONARPHONY_EXPORT
    #define SONARPHONY_LOCAL
  #endif
#endif
