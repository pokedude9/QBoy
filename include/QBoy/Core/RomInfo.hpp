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


#ifndef __QBOY_ROMINFO_HPP__
#define __QBOY_ROMINFO_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QString>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   30/04/2016
    /// \file   RomInfo.hpp
    /// \brief  Holds ROM header information and more.
    ///
    /// Public getters:
    /// const QString &path() const     [Ex: C:/Rom.gba]
    /// const QString &title() const    [Ex: POKEMON FIRE]
    /// const QString &version() const  [Ex: BPRE]
    /// bool isFRLG() const
    /// bool isRS() const
    /// bool isEM() const
    /// bool isValid() const
    /// bool isLoaded() const
    /// bool isExpanded() const
    ///
    ///////////////////////////////////////////////////////////
    class QBOY_API RomInfo {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Sets the class members either to NULL or to false.
        ///
        ///////////////////////////////////////////////////////////
        RomInfo();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Disables copying of the RomInfo for safety reasons.
        ///
        ///////////////////////////////////////////////////////////
        RomInfo(const RomInfo &info) = delete;



        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the file path of the rom on the disk.
        ///
        /// This value cannot be NULL upon saving. In that case, a
        /// dialog should be opened for the user to choose a new
        /// file path and saving the rom to it.
        ///
        /// \returns the absolute file path of the rom file.
        ///
        ///////////////////////////////////////////////////////////
        const QString &path() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the game title of the rom.
        ///
        /// The game title is always a 12-digit identifying code
        /// that usually represents the actual name of the game.
        /// A good example is POKEMON FIRE.
        ///
        /// \returns the 12-digit game title.
        ///
        ///////////////////////////////////////////////////////////
        const QString title() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the game version code of the rom.
        ///
        /// The game version code is extremely important to tools.
        /// It determines which data is loaded at which location
        /// and how it is interpreted afterwards.
        ///
        /// \returns the 4-digit game version code.
        ///
        ///////////////////////////////////////////////////////////
        const QString code() const;


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether this rom is a FR or LG one.
        ///
        /// Specific to Pokémon games. It compares whether the
        /// game version code (Ex: BPRE) starts with the sequence
        /// "BPR" or "BPG".
        ///
        /// \returns true if the ROM is either FireRed or LeafGreen.
        ///
        ///////////////////////////////////////////////////////////
        bool isFRLG() const;

        ///////////////////////////////////////////////////////////
        /// \brief Determines whether this rom is a R or S one.
        ///
        /// Specific to Pokémon games. It compares whether the
        /// game version code (Ex: AXVE) starts with the sequence
        /// "AXV" or "AXP".
        ///
        /// \returns true if the ROM is either Ruby or Sapphire.
        ///
        ///////////////////////////////////////////////////////////
        bool isRS() const;

        ///////////////////////////////////////////////////////////
        /// \brief Determines whether this rom is an emerald one.
        ///
        /// Specific to Pokémon games. It compares whether the
        /// game version code starts with the sequence "BPE".
        ///
        /// \returns true if the ROM is Emerald.
        ///
        ///////////////////////////////////////////////////////////
        bool isEM() const;


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether this rom is valid.
        ///
        /// A valid ROM has a valid header, to begin with, and
        /// occupies either 16MB or 32MB (not more, not less).
        ///
        /// \returns true if the current rom is valid.
        ///
        ///////////////////////////////////////////////////////////
        bool isValid() const;

        ///////////////////////////////////////////////////////////
        /// \brief Determines whether a rom is opened.
        ///
        /// This function is used to determine whether a rom has
        /// already been loaded. Apps can create a qboy::Rom object
        /// on the stack this way, without using a pointer to it and
        /// comparing it against NULL.
        ///
        /// \returns true if a rom is already opened.
        ///
        ///////////////////////////////////////////////////////////
        bool isLoaded() const;

        ///////////////////////////////////////////////////////////
        /// \brief Determines whether this rom is 32MB big.
        ///
        /// There are merely two kinds of GBA ROMs: Some occupy
        /// 16MB disk space only, others occupy a total of 32MB.
        /// [Ex(16): Pokemon FireRed; Ex(32): Pokemon Mystery Dungeon]
        ///
        /// \returns true if the rom size is 33554432.
        ///
        ///////////////////////////////////////////////////////////
        bool isExpanded() const;



        ///////////////////////////////////////////////////////////
        /// \brief Specifies the file path of the rom file.
        /// \param path Absolute file path to the rom
        ///
        ///////////////////////////////////////////////////////////
        void setPath(const QString &path);

        ///////////////////////////////////////////////////////////
        /// \brief Specifies the full 16-digit header string.
        ///
        /// Members IsFRLG, IsRS and IsEM will be automatically set
        /// through this function.
        ///
        /// \param code Must be a 16-digit string, else throws
        ///
        ///////////////////////////////////////////////////////////
        void setCode(const QString &code);


        ///////////////////////////////////////////////////////////
        /// \brief Specifies the validity of the rom.
        /// \param valid True if valid, false if invalid
        ///
        ///////////////////////////////////////////////////////////
        void setValid(bool valid);

        ///////////////////////////////////////////////////////////
        /// \brief Specifies the loading state of the rom.
        /// \param loaded True if rom is loaded, false if not
        ///
        ///////////////////////////////////////////////////////////
        void setLoaded(bool loaded);

        ///////////////////////////////////////////////////////////
        /// \brief Specifies the expanded state of the rom.
        /// \param expanded True if rom is expanded, false if not
        ///
        ///////////////////////////////////////////////////////////
        void setExpanded(bool expanded);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QString         m_Path;
        QString         m_Code;
        Boolean         m_IsFRLG;
        Boolean         m_IsRS;
        Boolean         m_IsEM;
        Boolean         m_IsValid;
        Boolean         m_IsLoaded;
        Boolean         m_IsExpanded;
    };


    ///////////////////////////////////////////////////////////
    /// \brief Defines common value types within the rom.
    ///
    ///////////////////////////////////////////////////////////
    enum ValueType : int
    {
        VT_None     = 0,
        VT_Byte     = 1,
        VT_HWord    = 2,
        VT_Word     = 4
    };
}


#endif  // __QBOY_ROMINFO_HPP__
