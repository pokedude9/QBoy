///////////////////////////////////////////////////////////
//
// QBoy: GameboyAdvance library
// Copyright (C) 2015-2016 Pokedude
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
///////////////////////////////////////////////////////////


#ifndef __QBOY_LZ77_HPP__
#define __QBOY_LZ77_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <Core/Rom.hpp>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   06/05/2016
    /// \file   Lz77.hpp
    /// \brief  Compresses or decompresses LZ77 data.
    ///
    ///////////////////////////////////////////////////////////
    class QBOY_API Lz77 {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Decompresses LZ77 data at the given offset.
        ///
        /// Attempts to decompress the LZ77 data at the specified
        /// offset and returns it in a QByteArray. Also outputs
        /// the size of the compressed data in order to implement
        /// repointing features.
        ///
        /// \param rom Rom to read LZ77 data from
        /// \param offset Offset to read data within rom from
        /// \param size Outputs the compressed data size
        /// \returns the uncompressed raw data.
        ///
        ///////////////////////////////////////////////////////////
        static QByteArray decompress(const Rom &rom, UInt32 offset, Int32 *size);

        ///////////////////////////////////////////////////////////
        /// \brief Compresses the given raw data to LZ77 data.
        ///
        /// Attempts to compress the LZ77 data and returns the
        /// compressed data in a QByteArray.
        ///
        /// \param array QByteArray to compress to LZ77 data
        /// \returns the compressed LZ77 data.
        ///
        ///////////////////////////////////////////////////////////
        static QByteArray compress(const QByteArray &raw);
    };
}


#endif  // __QBOY_LZ77_HPP__
