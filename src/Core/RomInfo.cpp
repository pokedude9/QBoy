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
#include <QBoy/Core/RomInfo.hpp>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    // Constructor
    //
    ///////////////////////////////////////////////////////////
    RomInfo::RomInfo()
        : m_Path(QString::null),
          m_Code(QString::null),
          m_IsFRLG(false),
          m_IsRS(false),
          m_IsEM(false),
          m_IsValid(false),
          m_IsLoaded(false),
          m_IsExpanded(false)
    {
    }


    ///////////////////////////////////////////////////////////
    // Public getters
    //
    ///////////////////////////////////////////////////////////
    const QString &RomInfo::path() const
    {
        return m_Path;
    }

    ///////////////////////////////////////////////////////////
    const QString RomInfo::title() const
    {
        // Extracts the first 12 characters
        Q_ASSERT(!m_Code.isNull());
        return m_Code.left(12);
    }

    ///////////////////////////////////////////////////////////
    const QString RomInfo::code() const
    {
        // Extracts the last 4 characters
        Q_ASSERT(!m_Code.isNull());
        return m_Code.right(4);
    }


    ///////////////////////////////////////////////////////////
    bool RomInfo::isFRLG() const
    {
        return m_IsFRLG;
    }

    ///////////////////////////////////////////////////////////
    bool RomInfo::isRS() const
    {
        return m_IsRS;
    }

    ///////////////////////////////////////////////////////////
    bool RomInfo::isEM() const
    {
        return m_IsEM;
    }


    ///////////////////////////////////////////////////////////
    bool RomInfo::isValid() const
    {
        return m_IsValid;
    }

    ///////////////////////////////////////////////////////////
    bool RomInfo::isLoaded() const
    {
        return m_IsLoaded;
    }


    ///////////////////////////////////////////////////////////
    // Public setters
    //
    ///////////////////////////////////////////////////////////
    void RomInfo::setPath(const QString &path)
    {
        Q_ASSERT(!path.isNull());
        m_Path = path;
    }

    ///////////////////////////////////////////////////////////
    void RomInfo::setCode(const QString &code)
    {
        Q_ASSERT(!code.isNull());
        m_Code = code;

        // Determines the game type
        m_IsFRLG =
            code.startsWith("BPR") ||
            code.startsWith("BPG");

        m_IsRS =
            code.startsWith("AXV") ||
            code.startsWith("AXP");

        m_IsEM =
            code.startsWith("BPE");
    }


    ///////////////////////////////////////////////////////////
    void RomInfo::setValid(bool valid)
    {
        m_IsValid = valid;
    }

    ///////////////////////////////////////////////////////////
    void RomInfo::setLoaded(bool loaded)
    {
        m_IsLoaded = loaded;
    }

    ///////////////////////////////////////////////////////////
    void RomInfo::setExpanded(bool expanded)
    {
        m_IsExpanded = expanded;
    }
}
