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


#ifndef __QBOY_CONFIG_HPP__
#define __QBOY_CONFIG_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QtCore/QtGlobal>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   30/04/2016
    /// \file   Config.hpp
    /// \brief  Exports or imports symbols from this library.
    ///
    /// Additionally, redefines integral types for readability.
    ///
    ///////////////////////////////////////////////////////////

    #   ifdef QBOY_LIBRARY
    #       define QBOY_API Q_DECL_EXPORT
    #   else
    #       define QBOY_API Q_DECL_IMPORT
    #   endif

    #   ifdef QT_DEBUG
    #       define QBOY_DEBUG
    #   else
    #       define QBOY_RELEASE
    #   endif
}


///////////////////////////////////////////////////////////
// Global type definitions
//
///////////////////////////////////////////////////////////
typedef unsigned char           UInt8;
typedef unsigned short          UInt16;
typedef unsigned int            UInt32;
typedef unsigned long long      UInt64;
typedef char                    Int8;
typedef short                   Int16;
typedef int                     Int32;
typedef long long               Int64;
typedef float                   Float;
typedef double                  Real;
typedef bool                    Boolean;



#endif  // __QBOY_CONFIG_HPP__
