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


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <Core/Lz77.hpp>
#include <QList>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    QByteArray Lz77::decompress(const Rom &rom, UInt32 offset, Int32 *size)
    {
        UInt8 *start = rom.data() + offset;
        UInt8 *array = rom.data() + offset;
        if (*array++ != 0x10)
            return QByteArray(NULL);

        // Instantiates a byte array
        QByteArray decomp;

        // Retrieves the length of the LZ77 data
        int position = 0;
        int length = (*(array++) + (*(array++) << 8) + (*(array++) << 16));
        decomp.resize(length);


        // Reads the LZ77 data and checks for compressed and uncompressed blocks
        while (position < length)
        {
            UInt8 isDecoded = *(array++);
            for (int i = 0; i < 8; i++)
            {
                if ((isDecoded & 0x80) != 0)
                {
                    int count = ((*(array) >> 4) + 3);
                    int depos = ((((*(array++) & 0xF) << 8) + *(array++)) + 1);
                    if (depos > length)
                        return QByteArray(NULL);

                    // Compressed data which needs to be decoded
                    for (int j = 0; j < count && position < length; j++)
                    {
                        decomp[position] = decomp.at((position-j)-depos+(j%depos));
                        position++;
                    }
                }
                else
                {
                    // Uncompressed data which needs to be copied
                    decomp[position++] = *(array++);
                }
                if (position >= length)
                {
                    break;
                }

                isDecoded <<= 1;
            }
        }


        // The compressed size is the current position minus the initial one
        size[0] = (array - start);
        return decomp;
    }
}
