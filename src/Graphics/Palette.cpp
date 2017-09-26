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
#include <QBoy/Core/Lz77.hpp>
#include <QBoy/Graphics/Palette.hpp>
#include <QBoy/Graphics/PaletteErrors.hpp>
#include <cmath>

namespace qboy
{
    ///////////////////////////////////////////////////////////
    // Constructors
    //
    ///////////////////////////////////////////////////////////
    Palette::Palette()
        : m_DataSize(0),
          m_ColorCount(0)
    {
    }

    ///////////////////////////////////////////////////////////
    Palette::Palette(const Palette &pal)
        : m_Data(pal.m_Data),
          m_DataGL(pal.m_DataGL),
          m_DataSize(pal.m_DataSize),
          m_ColorCount(pal.m_ColorCount)
    {
    }


    ///////////////////////////////////////////////////////////
    // Member functions
    //
    ///////////////////////////////////////////////////////////
    bool Palette::readUncompressed(const Rom &rom, UInt32 offset, Int32 count)
    {
        Q_ASSERT(count == 16 || count == 256);
        m_ColorCount = count;
        m_DataSize = count*2;


        // Attempts to seek to the given offset
        if (!rom.seek(offset) || !rom.canRead(m_DataSize))
        {
            m_LastError = PAL_ERROR_OFFSET;
            return false;
        }

        // Optimized: Reads all the half-words right away
        return convertGBA(rom.readHWordTable(count));
    }

    ///////////////////////////////////////////////////////////
    bool Palette::readCompressed(const Rom &rom, UInt32 offset)
    {
        // Determines whether the given offset is valid
        if (!rom.checkOffset(offset))
        {
            m_LastError = PAL_ERROR_OFFSET;
            return false;
        }

        // Attempts to decompress the LZ77 data
        QByteArray data = Lz77::decompress(rom, offset, &m_DataSize);
        if (data.isNull() || (data.size()/2 != 16 && data.size()/2 != 256))
        {
            m_LastError = PAL_ERROR_LZ77;
            return false;
        }


        // Defines required variables for the following algorithm
        m_ColorCount = (data.size() / 2);
        QList<UInt16> entries;

        // Converts the byte data to half-word data
        for (int i = 0; i < m_ColorCount; i++)
        {
            UInt8 nibble1 = data.at(i*2);
            UInt8 nibble2 = data.at(i*2+1);
            entries.push_back(static_cast<UInt16>((nibble2 << 8) | nibble1));
        }

        // Finally converts the GBA data to RGBA data
        return convertGBA(entries);
    }

    ///////////////////////////////////////////////////////////
    bool Palette::convertGBA(const QList<UInt16> &entries)
    {
        m_Data.clear();
        m_DataGL.clear();


        // Release mode will get to this point, even if invalid color count
        if (m_ColorCount != 16 && m_ColorCount != 256)
        {
            m_LastError = PAL_ERROR_COUNT;
            return false;
        }


        // Converts all color entries (internally)
        for (int i = 0; i < m_ColorCount; i++)
        {
            UInt16 entry = entries.at(i);
            UInt8 blue  = static_cast<UInt8>(((entry & 0x7C00) >> 0xA) * 8);
            UInt8 green = static_cast<UInt8>(((entry & 0x03E0) >> 0x5) * 8);
            UInt8 red   = static_cast<UInt8>(((entry & 0x001F) >> 0x0) * 8);

            m_Data.push_back({ red, green, blue, 255 });
            m_DataGL.push_back({ red/255.0f, green/255.0f, blue/255.0f, 1.0f });        
        }

        return true;
    }

    ///////////////////////////////////////////////////////////
    void Palette::convertRaw()
    {
        // Buffers the converted data
        for (int i = 0; i < m_ColorCount; i++)
        {
            const Color &entry = m_Data.at(i);
            char red = (char)(floorf(entry.r / 8.0f));
            char grn = (char)(floorf(entry.g / 8.0f));
            char blu = (char)(floorf(entry.b / 8.0f));

            // Converts the entry to a GBA color
            Int16 color = (Int16)(red | (grn << 5) | (blu << 10));
            m_Buffer.append((char)((color & 0xFF00) >> 8));
            m_Buffer.append((char)(color & 0xFF));
        }
    }


    ///////////////////////////////////////////////////////////
    const QVector<Color> &Palette::raw() const
    {
        return m_Data;
    }

    ///////////////////////////////////////////////////////////
    const QVector<GLColor> &Palette::rawGL() const
    {
        return m_DataGL;
    }


    ///////////////////////////////////////////////////////////
    const QString &Palette::lastError() const
    {
        return m_LastError;
    }


    ///////////////////////////////////////////////////////////
    bool Palette::setRaw(const QVector<Color> &raw)
    {
        // Determines whether the color count is correct
        if (raw.size() != 16 && raw.size() != 256)
        {
            m_LastError = PAL_ERROR_COUNT;
            return false;
        }

        // Copies the new color list
        m_Data = raw;
        m_DataGL.clear();
        m_ColorCount = raw.size();


        // Converts to OpenGL data list
        foreach (Color color, raw)
            m_DataGL.push_back({ color.r/255.0f, color.g/255.0f, color.b/255.0f, 1.0f });

        return true;
    }


    ///////////////////////////////////////////////////////////
    bool Palette::requiresRepoint(bool isCompressed)
    {
        // Converts the palette to raw byte data
        convertRaw();

        // Retrieves the new data size
        int newSize = 0;
        if (isCompressed)
            newSize = Lz77::compress(m_Buffer).size();
        else
            newSize = m_Buffer.size();

        return newSize > m_DataSize;
    }

    ///////////////////////////////////////////////////////////
    bool Palette::write(Rom &rom, UInt32 offset, Boolean lz77)
    {
        // Converts to raw data, if not already
        if (m_Buffer.isEmpty() || m_Buffer.isNull())
            convertRaw();

        // Converts the raw data to LZ77 data, if requested
        if (lz77)
        {
            m_Buffer = Lz77::compress(m_Buffer);

            if (m_Buffer.isNull() || m_Buffer.isEmpty())
            {
                m_LastError = PAL_ERROR_LZ77;
                return false;
            }
        }
        if (!rom.seek(offset))
        {
            m_LastError = PAL_ERROR_OFFSET;
            return false;
        }


        // Writes the palette to the ROM and clears the buffer
        rom.writeBytes(m_Buffer);
        m_Buffer.clear();

        return true;
    }
}
