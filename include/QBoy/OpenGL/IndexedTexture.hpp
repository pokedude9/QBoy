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


#ifndef __QBOY_INDEXEDTEXTURE_HPP__
#define __QBOY_INDEXEDTEXTURE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QBoy/Graphics/Color.hpp>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   09/05/2016
    /// \file   IndexedTexture.hpp
    /// \brief  Provides a palette and 8bpp image data.
    ///
    /// Creates and destroys OpenGL textures from 8bpp images.
    ///
    ///////////////////////////////////////////////////////////
    class QBOY_API IndexedTexture {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes an empty palette and null pixel data.
        ///
        ///////////////////////////////////////////////////////////
        IndexedTexture();

        ///////////////////////////////////////////////////////////
        /// \brief Default destructor
        ///
        /// Destroys the two used textures within OpenGL.
        ///
        ///////////////////////////////////////////////////////////
        ~IndexedTexture();


        ///////////////////////////////////////////////////////////
        /// \brief Sets the 8bpp pixel data for the texture.
        /// \param pixels Byte array containing the pixel data
        /// \param width Width of the image
        /// \param height Height of the image
        ///
        ///////////////////////////////////////////////////////////
        void setImage(UInt8 *pixels, Int32 width, Int32 height);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the palette data for the texture.
        /// \param palette Integer array containing the RGBA data
        ///
        ///////////////////////////////////////////////////////////
        void setPalette(GLColor *palette);


        ///////////////////////////////////////////////////////////
        /// \brief Updates 8bpp pixel data in a specific region.
        /// \param pixels Byte array containing the pixel data
        /// \param xpos X-position to update pixel data from
        /// \param ypos Y-position to update pixel data from
        /// \param width Width of the region to update
        /// \param height Height of the region to update
        ///
        ///////////////////////////////////////////////////////////
        void updateImage(UInt8 *pixels, Int32 xpos, Int32 ypos, Int32 width, Int32 height);

        ///////////////////////////////////////////////////////////
        /// \brief Updates one color entry within the palette
        /// \param color Color with R, G, B and A components
        /// \param index Palette index to replace
        ///
        ///////////////////////////////////////////////////////////
        void updateColor(const GLColor &color, UInt32 index);

        ///////////////////////////////////////////////////////////
        /// \brief Renders the current texture.
        /// \param point Position of the texture
        ///
        ///////////////////////////////////////////////////////////
        void render(const QPointF &point);


        ///////////////////////////////////////////////////////////
        /// \brief Sets the current OpenGL functions context.
        ///
        /// This is needed to provide rendering code in QBoy, as
        /// QBoy does not directly inherit QOpenGLFunctions.
        /// Additionally, initializes used OpenGL objects.
        ///
        ///////////////////////////////////////////////////////////
        void setOpenGLFunctions(QOpenGLFunctions *functions);

        ///////////////////////////////////////////////////////////
        /// \brief Sets the parental Qt OpenGL widget.
        ///
        ///////////////////////////////////////////////////////////
        void setParentWidget(QOpenGLWidget *widget);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        Int32               m_Width;
        Int32               m_Height;
        UInt32              m_PaletteID;
        UInt32              m_TextureID;
        UInt32              m_VertexBuffer;
        UInt32              m_IndexBuffer;
        GLColor            *m_Colors;
        UInt8              *m_Pixels;
        Float              *m_Buffer;
        QOpenGLFunctions   *m_Functions;
        QOpenGLWidget      *m_Parent;

        ///////////////////////////////////////////////////////////
        static QOpenGLVertexArrayObject *s_VertexArray;
        static QOpenGLShaderProgram     *s_ShaderProgram;
    };
}


#endif  // __QBOY_INDEXEDTEXTURE_HPP__
