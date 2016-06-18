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


#ifndef __QBOY_GLERRORS_HPP__
#define __QBOY_GLERRORS_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QOpenGLFunctions>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   24/05/2016
    /// \file   GLErrors.hpp
    /// \brief  Checks for errors within OpenGL.
    ///
    /// If in debug mode, checks for errors after every OpenGL.
    /// In release mode, evaluates to the expression to avoid
    /// having overhead, and as result, a low performance.
    ///
    ///////////////////////////////////////////////////////////
    class QBOY_API GLErrors : QOpenGLFunctions {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Initializes all the OpenGL functions.
        ///
        ///////////////////////////////////////////////////////////
        GLErrors() : QOpenGLFunctions()
        {
            initializeOpenGLFunctions();
        }

        ///////////////////////////////////////////////////////////
        /// \brief Checks whether OpenGL threw an error.
        ///
        /// Outputs a detailed error message with file, line and the
        /// causing expression in case an error occured.
        ///
        /// \param file Code file containing the expression
        /// \param line Line of the expression code
        /// \param expr The expression itself, as string
        ///
        ///////////////////////////////////////////////////////////
        void glCheckError(
            const char *file,
            const int   line,
            const char *expr
        );

        ///////////////////////////////////////////////////////////
        /// \brief Current GLErrors (OpenGLFunctions) object
        ///
        ///////////////////////////////////////////////////////////
        static GLErrors *Current;
    }; 


    #   ifdef QT_DEBUG
    #     define glCheck(expr) { expr; qboy::GLErrors::Current->glCheckError(__FILE__,__LINE__,#expr); }
    #   else
    #     define glCheck(expr) { expr; }
    #   endif


    ///////////////////////////////////////////////////////////
    /// \brief Shows a shader compilation error.
    ///
    /// Outputs a detailed error message that states the reasons
    /// why an OpenGL shader could not be compiled successfully.
    ///
    /// \param file Shader file containing the erroneous code
    /// \param msg  Contains the cause of the failure
    ///
    ///////////////////////////////////////////////////////////
    extern QBOY_API void glShaderError(
            const QString &file,
            const QString &msg
    );
}


#endif  // __QBOY_GLERRORS_HPP__
