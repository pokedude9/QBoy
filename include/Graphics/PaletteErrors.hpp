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


#ifndef __QBOY_PALETTEERRORS_HPP__
#define __QBOY_PALETTEERRORS_HPP__


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   08/05/2016
    /// \file   PaletteErrors.hpp
    /// \brief  Defines several error strings for palettes.
    ///
    ///////////////////////////////////////////////////////////

    #define PAL_ERROR_COUNT         "Palette count must be 16 or 256!"
    #define PAL_ERROR_OFFSET        "Given palette offset is out of rom range."
    #define PAL_ERROR_LZ77          "LZ77-data of the palette is invalid."
}


#endif  // __QBOY_PALETTEERRORS_HPP__
