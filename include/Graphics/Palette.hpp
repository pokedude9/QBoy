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


#ifndef __QBOY_PALETTE_HPP__
#define __QBOY_PALETTE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <Core/Rom.hpp>
#include <Graphics/Color.hpp>
#include <QVector>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   07/05/2016
    /// \file   Palette.hpp
    /// \brief  Reads uncompressed and compressed palettes.
    ///
    /// Reads LZ77 compressed and uncompressed palettes within
    /// the rom which consist of either 16 or 256 colors.
    ///
    ///////////////////////////////////////////////////////////
    class QBOY_API Palette {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of qboy::Palette.
        ///
        ///////////////////////////////////////////////////////////
        Palette();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all class members to another qboy::Palette.
        ///
        ///////////////////////////////////////////////////////////
        Palette(const Palette &pal);


        ///////////////////////////////////////////////////////////
        /// \brief Reads an uncompressed 16/256-color palette.
        ///
        /// Reads 32/512 bytes of raw palette data and converts
        /// them to 16/256 RGBA color entries.
        ///
        /// \param rom Currently active rom file
        /// \param offset Offset of the palette in the rom
        /// \param count Amount of colors to read. Must be 16/256!
        ///
        ///////////////////////////////////////////////////////////
        bool readUncompressed(const Rom &rom, UInt32 offset, Int32 count);

        ///////////////////////////////////////////////////////////
        /// \brief Reads an compressed 16/256-color palette.
        ///
        /// Reads LZ77-compressed palette data and converts
        /// them to 16/256 RGBA color entries.
        ///
        /// \param rom Currently active rom file
        /// \param offset Offset of the palette in the rom
        ///
        ///////////////////////////////////////////////////////////
        bool readCompressed(const Rom &rom, UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the actual RGBA data of the palette.
        ///
        ///////////////////////////////////////////////////////////
        const QVector<Color> &raw() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the OpenGL floating-point data.
        ///
        ///////////////////////////////////////////////////////////
        const QVector<GLColor> &rawGL() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the last error upon loading or writing.
        /// \returns the last error (no stacking!).
        ///
        ///////////////////////////////////////////////////////////
        const QString &lastError() const;


        ///////////////////////////////////////////////////////////
        /// \brief Specifies the new color table of this palette.
        ///
        /// The given vector must either contain 16 or 256 colors.
        /// An error will be thrown if this condition is not met.
        ///
        /// \param raw Raw color vector, sized 16 or 256
        ///
        ///////////////////////////////////////////////////////////
        bool setRaw(const QVector<Color> &raw);


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether repointing is required.
        /// \param isCompressed Is the image to be written LZ77?
        /// \returns true if the image must be repointed
        ///
        ///////////////////////////////////////////////////////////
        bool requiresRepoint(bool isCompressed);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the palette to the given offset.
        /// \param rom Currently opened ROM file
        /// \param offset Offset to write palette to
        /// \param lz77 Should palette be LZ77-encoded?
        /// \returns false if an error occured.
        ///
        ///////////////////////////////////////////////////////////
        bool write(Rom &rom, UInt32 offset, Boolean lz77 = false);


    protected:

        ///////////////////////////////////////////////////////////
        /// \brief Converts raw GBA color data to RGBA data.
        /// \param array List of hwords containing the GBA entries
        ///
        ///////////////////////////////////////////////////////////
        bool convertGBA(const QList<UInt16> &entries);

        ///////////////////////////////////////////////////////////
        /// \brief Converts the raw RGBA data to GBA color data.
        /// \returns the encoded byte array of the data.
        ///
        ///////////////////////////////////////////////////////////
        void convertRaw();


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QVector<Color>      m_Data;
        QVector<GLColor>    m_DataGL;
        Int32               m_DataSize;
        Int32               m_ColorCount;
        QString             m_LastError;
        QByteArray          m_Buffer;
   };
}


#endif  // __QBOY_PsALETTE_HPP__
