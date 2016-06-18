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


#ifndef __QBOY_IMAGE_HPP__
#define __QBOY_IMAGE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Graphics/Palette.hpp>
#include <QBoy/OpenGL/IndexedTexture.hpp>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   08/05/2016
    /// \file   Image.hpp
    /// \brief  Reads pixel data in 4bpp and 8bpp format.
    ///
    /// Reads and writes images from/to rom and creates OpenGL
    /// components to render these images with a palette.
    ///
    ///////////////////////////////////////////////////////////
    class QBOY_API Image {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of qboy::Image.
        ///
        ///////////////////////////////////////////////////////////
        Image();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies the raw pixel data plus the current palette.
        ///
        ///////////////////////////////////////////////////////////
        Image(const Image &img);


        ///////////////////////////////////////////////////////////
        /// \brief Returns the last error beeing thrown.
        ///
        ///////////////////////////////////////////////////////////
        const QString &lastError() const;


        ///////////////////////////////////////////////////////////
        /// \brief Reads an uncompressed image from the rom.
        /// \param rom Currently active rom instance
        /// \param offset Offset of the image within the rom
        /// \param length Total length of the data
        /// \param width Width of the resulting image
        /// \returns the success of the procedure.
        ///
        ///////////////////////////////////////////////////////////
        bool readUncompressed(
                const Rom &rom,
                UInt32 offset,
                Int32 length,
                Int32 width,
                Boolean is4bpp
        );

        ///////////////////////////////////////////////////////////
        /// \brief Reads a compressed image from the rom.
        /// \param rom Currently active rom instance
        /// \param offset Offset of the image within the rom
        /// \param width Width of the resulting image
        /// \returns the success of the procedure.
        ///
        ///////////////////////////////////////////////////////////
        bool readCompressed(
                const Rom &rom,
                UInt32 offset,
                Int32 width,
                Boolean is4bpp
        );

        // TODO: write

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the raw 8bpp pixel data.
        ///
        /// Even if attempting to load a 4bpp image, will be
        /// converted to a 8bpp one for compatibility with OpenGL.
        ///
        /// \returns the raw 8bpp (index) data.
        ///
        ///////////////////////////////////////////////////////////
        const QByteArray &raw() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the associated palette.
        /// \returns the associated qboy::Palette pointer.
        ///
        ///////////////////////////////////////////////////////////
        const Palette *palette() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the size of the image in pixels.
        /// \returns the pixel size of the image
        ///
        ///////////////////////////////////////////////////////////
        QSize size() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the generated 8bpp OpenGL texture.
        ///
        /// This function should not be called if a palette has
        /// not yet been specified (qboy::Image::setPalette).
        ///
        ///////////////////////////////////////////////////////////
        const IndexedTexture *texture(QOpenGLFunctions *funcs, QOpenGLWidget *parent) const;


        ///////////////////////////////////////////////////////////
        /// \brief Specifies the raw 8bpp pixel data.
        ///
        /// Given pixel data must be in 8bpp format. If the palette
        /// does only contain 16 entries, it will be automatically
        /// converted to a 4bpp image. Exception safe!
        ///
        /// \param raw Raw 8bpp data
        ///
        ///////////////////////////////////////////////////////////
        void setRaw(const QByteArray &raw);

        ///////////////////////////////////////////////////////////
        /// \brief Specifies the size of the image.
        ///
        /// Do not call this function after reading an image!
        ///
        /// \param width Width of the image in pixels
        /// \param height Height of the image in pixels
        /// \returns false if width or height are not a multiple of 8.
        ///
        ///////////////////////////////////////////////////////////
        bool setSize(Int32 width, Int32 height);

        ///////////////////////////////////////////////////////////
        /// \brief Specifies the palette of the image.
        /// \param palette Pointer to valid palette structure.
        ///
        ///////////////////////////////////////////////////////////
        void setPalette(Palette *palette);


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether the image requires a repoint.
        /// \param isCompressed Should image be LZ77-compressed?
        ///
        ///////////////////////////////////////////////////////////
        bool requiresRepoint(bool isCompressed);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the image to ROM.
        /// \param rom Currently opened ROM file
        /// \param offset Offset to write image to
        /// \param isLz77 Should image be LZ77-compressed?
        /// \returns false if image could not be compressed.
        ///
        ///////////////////////////////////////////////////////////
        bool write(Rom &rom, UInt32 offset, Boolean isLz77);



    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Converts the raw data to GBA index data.
        ///
        ///////////////////////////////////////////////////////////
        void convertToGBA();


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QByteArray          m_Data;
        Palette            *m_Palette;
        Int32               m_DataSize;
        Int32               m_Width;
        Int32               m_Height;
        Boolean             m_Is4Bpp;
        QByteArray          m_Buffer;
        QString             m_LastError;
    };
}


#endif  // __QBOY_IMAGE_HPP__
