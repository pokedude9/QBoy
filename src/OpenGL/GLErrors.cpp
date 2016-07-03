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
#include <QBoy/OpenGL/GLErrors.hpp>
#include <QOpenGLFunctions>
#include <iostream>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    // Static objects
    //
    ///////////////////////////////////////////////////////////
    GLErrors *GLErrors::Current = NULL;


    ///////////////////////////////////////////////////////////
    // Global functions
    //
    ///////////////////////////////////////////////////////////
    void GLErrors::glCheckError(const char *file, const int line, const char *expr)
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::string str_file = file;
            std::string str_emsg = "Unknown error";
            std::string str_desc = "No description";

            // Retrieves the error string for each OpenGL error
            switch (error) {
            case GL_INVALID_ENUM:
                str_emsg = "Invalid enumerator";
                str_desc = "Specified enum value is not valid for this function.";
                break;

            case GL_INVALID_INDEX:
                str_emsg = "Invalid index";
                str_desc = "Specified index is either negative or out of range.";
                break;

            case GL_INVALID_VALUE:
                str_emsg = "Invalid identifier";
                str_desc = "Specified value is not a valid OpenGL identifier for this function.";
                break;

            case GL_INVALID_OPERATION:
                str_emsg = "Invalid operation";
                str_desc = "Specified operation could not be performed due to wrong states.";
                break;

            case GL_OUT_OF_MEMORY:
                str_emsg = "Out of memory";
                str_desc = "The specified OpenGL object could not be allocated due to low memory.";
            }


            // Outputs the detailed error string
            std::cout << "The following expression caused an OpenGL error in file \""
                      << str_file.substr(str_file.find_last_of("\\/") + 1)
                      << "\" in line " << line << ".\n\n"
                      << "Function call:\n\n\t" << expr
                      << "\n\nDetails:\n\n\t" << str_emsg
                      << "\n\t" << str_desc << "\n\n";
        }
    }

    ///////////////////////////////////////////////////////////
    void glShaderError(const QString &file, const QString &msg)
    {
        // Outputs the detailed error string
        std::cout << "The following shader could not be compiled successfully:\n\n\t"
                  << file.toStdString() << "\n\nDetailed description:\n\n\t"
                  << msg.toStdString() << "\n\n";
    }
}
