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
#include <QBoy/Graphics/Image.hpp>
#include <QBoy/Graphics/ImageErrors.hpp>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    // Constructor and destructor
    //
    ///////////////////////////////////////////////////////////
    Image::Image()
        : m_Palette(0),
          m_DataSize(0),
          m_Width(0),
          m_Height(0),
          m_Is4Bpp(false)
    {
    }

    ///////////////////////////////////////////////////////////
    Image::Image(const Image &img)
    {
        m_Data = img.m_Data;
        m_Palette = img.m_Palette;
        m_DataSize = img.m_DataSize;
    }


    ///////////////////////////////////////////////////////////
    // Member functions
    //
    ///////////////////////////////////////////////////////////
    const QString &Image::lastError() const
    {
        return m_LastError;
    }

    ///////////////////////////////////////////////////////////
    bool Image::readUncompressed(
            const Rom &rom,
            UInt32 offset,
            Int32 length,
            Int32 width,
            Boolean is4bpp
    )
    {
        if (!rom.seek(offset))
        {
            m_LastError = IMG_ERROR_OFFSET;
            return false;
        }


        // If the data is 4bpp, it needs to be widened for the OpenGL
        // shader and for faster pixel access in general.
        if (is4bpp)
        {
            // Reads the uncompressed data and assures it is valid
            QByteArray data = rom.readBytes(length);
            if (width % 8 != 0 || length % 2 != 0 || data.isNull())
            {
                m_LastError = IMG_ERROR_LENGTH;
                return false;
            }

            // Calculates the height, given by width and data length
            float ratio = (float)((length / 32.0f) / (width / 8.0f));
            m_Height = (int)(ceilf(ratio))*8;
            m_Width = width;
            m_DataSize = length;
            m_Data.resize(m_Width*m_Height);

            // Converts the 4bpp data to 8bpp data. Extracts the two nibbles out
            // of one byte in each iteration and treats them as indices.
            int pixelInc = 0;
            for (int y = 0; y < m_Height-7; y+=8)
                for (int x = 0; x < width-7; x+=8)
                    for (int y2 = 0; y2 < 8; y2++)
                        for (int x2 = 0; x2 < 8; x2+=2)
                        {
                            UInt8 nibbles = data.at(pixelInc++);
                            m_Data[(x+x2+1)+((y+y2)*width)] = (Int8)((nibbles & 0xF0) >> 4);
                            m_Data[(x+x2+0)+((y+y2)*width)] = (Int8)((nibbles & 0x0F));
                        }
        }
        else
        {
            // Reads the uncompressed data and assures it is valid
            m_Data = rom.readBytes(length);
            if (width % 8 != 0 || length % 2 != 0 || m_Data.isNull())
            {
                m_LastError = IMG_ERROR_LENGTH;
                return false;
            }
        }


        m_Is4Bpp = is4bpp;
        return true;
    }

    ///////////////////////////////////////////////////////////
    bool Image::readCompressed(
            const Rom &rom,
            UInt32 offset,
            Int32 width,
            Boolean is4bpp
    )
    {
        if (!rom.seek(offset))
        {
            m_LastError = IMG_ERROR_OFFSET;
            return false;
        }


        // If the data is 4bpp, it needs to be widened for the OpenGL
        // shader and for faster pixel access in general.
        if (is4bpp)
        {
            // Decompresses the LZ77 data and assures it is valid
            QByteArray data = Lz77::decompress(rom, offset, &m_DataSize);
            if (width % 8 != 0 || data.size() % 2 != 0)
            {
                m_LastError = IMG_ERROR_LENGTH;
                return false;
            }
            else if (data.isNull())
            {
                m_LastError = IMG_ERROR_LZ77;
                return false;
            }

            // Calculates the height, given by width and data length
            float ratio = (float)((data.size() / 32.0f) / (width / 8.0f));
            m_Height = (int)(ceilf(ratio))*8;
            m_Width = width;
            m_Data.resize(m_Width*m_Height);

            // Converts the 4bpp data to 8bpp data. Extracts the two nibbles out
            // of one byte in each iteration and treats them as indices.
            int pixelInc = 0;
            for (int y = 0; y < m_Height-7; y+=8)
                for (int x = 0; x < width-7; x+=8)
                    for (int y2 = 0; y2 < 8; y2++)
                        for (int x2 = 0; x2 < 8; x2+=2)
                        {
                            if (pixelInc == data.size())
                                goto end;

                            UInt8 nibbles = data.at(pixelInc++);
                            m_Data[(x+x2+1)+((y+y2)*width)] = (Int8)((nibbles & 0xF0) >> 4);
                            m_Data[(x+x2+0)+((y+y2)*width)] = (Int8)((nibbles & 0x0F));
                        }
        }
        else
        {
            m_Data = Lz77::decompress(rom, offset, &m_DataSize);
            if (width % 8 != 0 || m_Data.size() % 2 != 0)
            {
                m_LastError = IMG_ERROR_LENGTH;
                return false;
            }
            else if (m_Data.isNull())
            {
                m_LastError = IMG_ERROR_LZ77;
                return false;
            }

            // Calculates the height, given by width and data length
            float ratio = (float)((m_Data.size() / 32.0f) / (width / 8.0f));
            m_Height = (int)(ceilf(ratio))*8;
            m_Width = width;
        }

    end:
        m_Is4Bpp = is4bpp;
        return true;
    }

    ///////////////////////////////////////////////////////////
    const IndexedTexture *Image::texture(QOpenGLFunctions *funcs, QOpenGLWidget *parent) const
    {
        IndexedTexture *tex = new IndexedTexture;
        tex->setOpenGLFunctions(funcs);
        tex->setParentWidget(parent);
        tex->setImage((UInt8*)m_Data.data(), m_Width, m_Height);
        tex->setPalette((GLColor*)m_Palette->rawGL().data());


        // Returns the texture; widget and functions must be specified manually
        return tex;
    }


    ///////////////////////////////////////////////////////////
    bool Image::requiresRepoint(bool isCompressed)
    {
        // Converts the image to GBA index data first
        convertToGBA();

        // Retrieves the new size of the image
        int newSize = 0;
        if (isCompressed)
            newSize = Lz77::compress(m_Buffer).size();
        else
            newSize = m_Buffer.size();


        return newSize > m_DataSize;
    }

    ///////////////////////////////////////////////////////////
    bool Image::write(Rom &rom, UInt32 offset, Boolean isLz77)
    {
        // Converts the image to GBA data, if not already
        if (m_Buffer.isNull() || m_Buffer.isEmpty())
            convertToGBA();

        // Compresses the buffer, if requested
        if (isLz77)
        {
            m_Buffer = Lz77::compress(m_Buffer);

            if (m_Buffer.isNull() || m_Buffer.isEmpty())
            {
                m_LastError = IMG_ERROR_LZ77;
                return false;
            }
        }
        if (!rom.seek(offset))
        {
            m_LastError = IMG_ERROR_OFFSET;
            return false;
        }


        // Writes the image to ROM and clears the buffer
        rom.writeBytes(m_Buffer);
        m_Buffer.clear();

        return true;
    }


    ///////////////////////////////////////////////////////////
    // Protected methods
    //
    ///////////////////////////////////////////////////////////
    void Image::convertToGBA()
    {
        // Combines two consecutive indices into one byte
        for (int y = 0; y < m_Height-7; y+=8)
        {
            for (int x = 0; x < m_Width-7; x+=8)
            {
                for (int y2 = 0; y2 < 8; y2++)
                {
                    for (int x2 = 0; x2 < 8; x2+=2)
                    {
                        Int8 pixel1 = m_Data[(x+x2+0)+((y+y2)*m_Height)];
                        Int8 pixel2 = m_Data[(x+x2+1)+((y+y2)*m_Height)];
                        m_Buffer.push_back((Int8)((pixel2 << 4) | pixel1));
                    }
                }
            }
        }
    }


    ///////////////////////////////////////////////////////////
    // Getters and setters
    //
    ///////////////////////////////////////////////////////////
    const QByteArray &Image::raw() const
    {
        return m_Data;
    }

    ///////////////////////////////////////////////////////////
    QSize Image::size() const
    {
        return QSize(m_Width, m_Height);
    }

    ///////////////////////////////////////////////////////////
    const Palette *Image::palette() const
    {
        return m_Palette;
    }

    ///////////////////////////////////////////////////////////
    void Image::setRaw(const QByteArray &raw)
    {
        m_Data = raw;
    }

    ///////////////////////////////////////////////////////////
    void Image::setPalette(Palette *palette)
    {
        m_Palette = palette;
    }

    ///////////////////////////////////////////////////////////
    bool Image::setSize(Int32 width, Int32 height)
    {
        if (width % 8 != 0 || height % 8 != 0)
            return false;

        m_Width = width;
        m_Height = height;
        return true;
    }
}
