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

    ///////////////////////////////////////////////////////////
    int *attempt_encode(Int8 *ptr, int pos, int len) // always 2 entries
    {
        // Checks whether the following data can be encoded
        if (pos >= len)
            return new int[2] { -1, 0 };
        if ((pos < 2) || (len-pos) < 2)
            return new int[2] { 0, 0 };

        // Encodes the block
        QList<int> l_pos;
        for (int i = 1; (i < 4096) && (i < pos); i++)
            if (ptr[pos-(i+1)] == ptr[pos])
               l_pos.append(i+1);

        if (l_pos.size() == 0)
            return new int[2] { 0, 0 };


       int count = 0;
       bool islz = true;
       while (count++ < 18 && islz)
       {
           for (int i = l_pos.size()-1; i >= 0; i--)
           {
               if (ptr[pos+count] != ptr[(pos-l_pos[i])+(count%l_pos[i])])
                   l_pos.removeAt(i);
               else
                   islz = false;
           }
       }

       return new int[2] { count, l_pos[0] };
    }

    ///////////////////////////////////////////////////////////
    QByteArray Lz77::compress(const QByteArray &raw)
    {
        QByteArray encoded;
        encoded.append(0x10);
        int position = 0;
        int size = raw.size();

        // Computes the data length
        Int8 *pSize = (Int8*)(&size);
        encoded.append(*pSize++);
        encoded.append(*pSize++);
        encoded.append(*pSize++);


        // Encodes the data in blocks
        while (position < size)
        {
            QByteArray block;
            Int8 bEnc = 0;

            for (int j = 0; j < 8; j++)
            {
                int *row = attempt_encode(raw.data(), position, size);
                if (row[0] > 2)
                {
                    // Can be Lz77 encoded
                    Int8 lo = (Int8)((((row[0]-3)&0xF)<<4)+(((row[1]-1)>>8)&0xF));
                    Int8 hi = (Int8)((row[1]-1)&0xFF);

                    block.append(lo);
                    block.append(hi);
                    position += row[0];
                    bEnc |= (Int8)(1 << (8-(j+1)));
                }
                else if (row[0] >= 0)
                {
                    // Can not be encoded; raw data
                    block.append(array[position++]);
                }
                else
                {
                    // No data to proceed
                    break;
                }
            }

            encoded.append(bEnc);
            encoded.append(block);
        }

        // Aligns the Lz77 data length to four
        while (encoded.size() % 4 != 0)
            encoded.append(0);

        return encoded;
    }
}
