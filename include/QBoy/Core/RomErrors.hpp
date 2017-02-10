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


#ifndef __QBOY_ROMERRORS_HPP__
#define __QBOY_ROMERRORS_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QString>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   03/05/2016
    /// \file   RomErrors.hpp
    /// \brief  Defines errors concerning the rom files.
    ///
    ///////////////////////////////////////////////////////////

    #define ROM_ERROR_FNF       "The ROM file was not found: \"%file%\"."
    #define ROM_ERROR_IO        "The ROM file is already in use: \"%file%\"."
    #define ROM_ERROR_SIZE      "The ROM file is not a proper size (should be either 16MB or 32MB)."


    ///////////////////////////////////////////////////////////
    /// \brief Converts an error regarding files and paths.
    ///
    ///////////////////////////////////////////////////////////
    inline QString convertFileError(const char *str, const QString &file)
    {
        QString error(str);
        return QString(error.replace("%file%", file));
    }
}


#endif  // __QBOY_ROMERRORS_HPP__
