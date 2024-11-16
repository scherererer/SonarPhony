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

#define _U2S(b) reinterpret_cast<char const *> (b)

namespace sonarphony
{

/*enum
{

	O_SYNC          = 0,
	O_SIZE          = 4,
	O_STATUS        = 6,
	O_TYPE          = 10,   // 2 bytes

};*/

inline char calculateChecksum (QByteArray const &buffer_)
{
	char sum = 0;

	for (int i = 0; i < buffer_.size (); ++i)
		sum += static_cast<char> (buffer_[i]);

	return sum;
}

}
