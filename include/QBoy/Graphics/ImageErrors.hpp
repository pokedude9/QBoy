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


#ifndef __QBOY_IMAGEERRORS_HPP__
#define __QBOY_IMAGEERRORS_HPP__


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   28/05/2016
    /// \file   ImageErrors.hpp
    /// \brief  Defines several error strings for images.
    ///
    ///////////////////////////////////////////////////////////

    #define IMG_ERROR_OFFSET    "Given image offset is out of rom range."
    #define IMG_ERROR_LZ77      "The LZ77 data of the image is broken."
    #define IMG_ERROR_LENGTH    "The length is not a multiple of 2 or the width is not a multiple of 8."
}


#endif  // __QBOY_IMAGEERRORS_HPP__
