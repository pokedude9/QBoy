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
#include <OpenGL/GLErrors.hpp>
#include <OpenGL/IndexedTexture.hpp>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    // Definitions
    //
    ///////////////////////////////////////////////////////////
    #define IT_VERTEX_ATTR      0
    #define IT_COORD_ATTR       1

    ///////////////////////////////////////////////////////////
    // Static variables
    //
    ///////////////////////////////////////////////////////////
    QOpenGLVertexArrayObject *IndexedTexture::s_VertexArray;
    QOpenGLShaderProgram     *IndexedTexture::s_ShaderProgram;


    ///////////////////////////////////////////////////////////
    // Constructor and destructor
    //
    ///////////////////////////////////////////////////////////
    IndexedTexture::IndexedTexture()
        : m_Width(0),
          m_Height(0),
          m_PaletteID(0),
          m_TextureID(0),
          m_VertexBuffer(0),
          m_IndexBuffer(0),
          m_Colors(0),
          m_Pixels(0)
    {
    }

    ///////////////////////////////////////////////////////////
    IndexedTexture::~IndexedTexture()
    {
        // Disposes of all OpenGL objects used by this class instance
        glCheck(m_Functions->glDeleteTextures(1, &m_PaletteID));
        glCheck(m_Functions->glDeleteTextures(1, &m_TextureID));
        glCheck(m_Functions->glDeleteBuffers(1, &m_VertexBuffer));
        glCheck(m_Functions->glDeleteBuffers(1, &m_IndexBuffer));

        // Frees the pixel and color data
        delete m_Colors;
        delete m_Pixels;
    }


    ///////////////////////////////////////////////////////////
    // Setters
    //
    ///////////////////////////////////////////////////////////
    void IndexedTexture::setPalette(GLColor *palette)
    {
        m_Colors = palette;

        // Fills the OpenGL palette texture with data
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_PaletteID));
        glCheck(m_Functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_FLOAT, palette));
    }

    ///////////////////////////////////////////////////////////
    void IndexedTexture::setImage(UInt8 *pixels, Int32 width, Int32 height)
    {
        m_Width = width;
        m_Height = height;
        m_Pixels = pixels;

        // Converts the width and height to float for OpenGL
        float ogl_w = (float)width;
        float ogl_h = (float)height;

        // Allocates space with initial texture data
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_TextureID));
        glCheck(m_Functions->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels));

        // Updates the vertex-buffer
        m_Buffer = new float[16]
        {
           // Position        Coords
           // X      Y        U    V
              0,     0,       0,   0,
              ogl_w, 0,       1,   0,
              ogl_w, ogl_h,   1,   1,
              0,     ogl_h,   0,   1
        };

        // Writes the vertex-buffer
        glCheck(m_Functions->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        glCheck(m_Functions->glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), m_Buffer, GL_DYNAMIC_DRAW));
    }


    ///////////////////////////////////////////////////////////
    // Member functions
    //
    ///////////////////////////////////////////////////////////
    void IndexedTexture::updateImage(UInt8 *pixels, Int32 xpos, Int32 ypos, Int32 width, Int32 height)
    {
        // Updates only if rectangle is valid
        if (xpos + width  > m_Width  ||
            ypos + height > m_Height ||
            xpos < 0 || ypos < 0)
            return;


        // Updates the pixels of a specific region.
        int pixelInc = 0;
        for (int y = ypos; y < height; y++)
            for (int x = xpos; x < width; x++)
                m_Pixels[x+y*m_Width] = pixels[pixelInc++];

        // Applies the updated data to the current texture
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_TextureID));
        glCheck(m_Functions->glTexSubImage2D(GL_TEXTURE_2D, 0, xpos, ypos, width, height, GL_RED, GL_UNSIGNED_BYTE, pixels));
    }

    ///////////////////////////////////////////////////////////
    void IndexedTexture::updateColor(const GLColor &color, UInt32 index)
    {
        // Updates only if index is valid
        if (index > 256)
            return;

        // Replaces old color entry
        m_Colors[index] = color;

        // Applies the updated color to the texture
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_PaletteID));
        glCheck(m_Functions->glTexSubImage2D(GL_TEXTURE_2D, 0, index, 0, 1, 1, GL_RGBA, GL_FLOAT, &color));
    }

    ///////////////////////////////////////////////////////////
    void IndexedTexture::render(const QPointF &point)
    {
        // Fetches size of the parental widget
        Float dim_w = (Float)m_Parent->width();
        Float dim_h = (Float)m_Parent->height();

        // Computes the MVP matrix
        QMatrix4x4 mat_mvp;
        mat_mvp.setToIdentity();
        mat_mvp.ortho(0, dim_w, dim_h, 0, -1, 1);
        mat_mvp.translate(point.x(), point.y());

        // Binds the vertex array, vertex buffer and index buffer
        glCheck(s_VertexArray->bind());
        glCheck(m_Functions->glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        glCheck(m_Functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));

        // Specifies the matrix and the buffers within the shader program
        s_ShaderProgram->bind();
        s_ShaderProgram->setUniformValue("uni_mvp", mat_mvp);
        s_ShaderProgram->enableAttributeArray(IT_VERTEX_ATTR);
        s_ShaderProgram->enableAttributeArray(IT_COORD_ATTR);
        s_ShaderProgram->setAttributeBuffer(IT_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 4*sizeof(float));
        s_ShaderProgram->setAttributeBuffer(IT_COORD_ATTR,  GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));

        // Binds the textures to their respective units
        glCheck(m_Functions->glActiveTexture(GL_TEXTURE1));
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_PaletteID));
        glCheck(m_Functions->glActiveTexture(GL_TEXTURE0));
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_TextureID));

        // Draws six triangular elements (i.e. a quad texture)
        glCheck(m_Functions->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
    }


    ///////////////////////////////////////////////////////////
    // Misc functions
    //
    ///////////////////////////////////////////////////////////
    void IndexedTexture::setOpenGLFunctions(QOpenGLFunctions *functions)
    {
        m_Functions = functions;

        // Initializes shared OpenGL objects
        if (!s_VertexArray)
        {
            s_VertexArray = new QOpenGLVertexArrayObject;
            s_VertexArray->create();
            s_VertexArray->bind();
        }
        if (!s_ShaderProgram)
        {
            s_ShaderProgram = new QOpenGLShaderProgram;
            s_ShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/IndexedVertexShader.glsl");
            s_ShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/IndexedFragmentShader.glsl");
            s_ShaderProgram->link();
            s_ShaderProgram->bind();
            s_ShaderProgram->setUniformValue("smp_texture", 0);
            s_ShaderProgram->setUniformValue("smp_palette", 1);
        }
    }

    ///////////////////////////////////////////////////////////
    void IndexedTexture::setParentWidget(QOpenGLWidget *widget)
    {
        m_Parent = widget;

        // Initializes private OpenGL objects
        glCheck(m_Functions->glGenBuffers(1, &m_VertexBuffer));
        glCheck(m_Functions->glGenBuffers(1, &m_IndexBuffer));
        glCheck(m_Functions->glGenTextures(1, &m_PaletteID));
        glCheck(m_Functions->glGenTextures(1, &m_TextureID));
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_PaletteID));
        glCheck(m_Functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(m_Functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(m_Functions->glBindTexture(GL_TEXTURE_2D, m_TextureID));
        glCheck(m_Functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(m_Functions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        // The index buffer is uniform; thus initialize and store it now
        const unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
        glCheck(m_Functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        glCheck(m_Functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned), indices, GL_STATIC_READ));
    }
}
