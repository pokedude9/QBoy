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


#ifndef __QBOY_ROM_HPP__
#define __QBOY_ROM_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/RomInfo.hpp>
#include <QByteArray>
#include <QList>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   30/04/2016
    /// \file   Rom.hpp
    /// \brief  Wraps a rom file and provides useful functions.
    ///
    /// Public I/O members:
    /// UInt8 readByte() const
    /// UInt16 readHWord() const
    /// UInt32 readWord() const
    /// UInt32 readPointer() const
    /// QByteArray readBytes(UInt32 count) const
    /// QList<UInt16> readHWordTable(Int32 count) const
    /// QList<UInt32> readWordTable(Int32 count) const
    /// QList<UInt32> readPointerTable(Int32 count) const
    /// void writeByte(UInt8 byte)
    /// void writeHWord(UInt16 hword)
    /// void writeWord(UInt32 word)
    /// void writePointer(UInt32 offset)
    /// void writeBytes(const QByteArray &bytes)
    /// void writeHWordTable(const QList<UInt16> &hwords)
    /// void writeWordTable(const QList<UInt32> &words)
    /// void writePointerTable(const QList<Uint32> &offsets)
    ///
    ///////////////////////////////////////////////////////////
    class QBOY_API Rom {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of qboy::Rom. Designed to
        /// be created on the stack (local references).
        ///
        ///////////////////////////////////////////////////////////
        Rom();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copying of qboy::Rom objects is disabled due to safety.
        ///
        ///////////////////////////////////////////////////////////
        Rom(const Rom &rom) = delete;

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Disposes of any resources not freed by qboy::Rom::close.
        ///
        ///////////////////////////////////////////////////////////
        ~Rom();


        ///////////////////////////////////////////////////////////
        /// \brief Loads a rom located at a given file path.
        ///
        /// Reads all byte contents of the rom and returns control
        /// to the operating system immediately. Will return false
        /// if loading fails. Corresponding error string can be
        /// retrieved by using qboy::Rom::lastError.
        ///
        /// \param path Absolute file path to the rom
        ///
        ///////////////////////////////////////////////////////////
        bool loadFromFile(const QString &path);

        ///////////////////////////////////////////////////////////
        /// \brief Releases all resources used by qboy::Rom.
        ///
        /// Will discard any changes made to the rom between now
        /// and the time at which qboy::Rom::save was called last.
        ///
        ///////////////////////////////////////////////////////////
        void close();


        ///////////////////////////////////////////////////////////
        /// \brief Saves all byte changes to the original file.
        ///
        /// Writes the whole byte blob to the original file path.
        /// Additionally, if existing, loops through all the added
        /// qboy::RomEntry instances and also writes them to rom.
        ///
        ///////////////////////////////////////////////////////////
        bool save();

        ///////////////////////////////////////////////////////////
        /// \brief Saves all byte changes to another file.
        ///
        /// Modifies the path of the qboy::RomInfo structure and
        /// calls the member function qboy::Rom::save afterwards.
        ///
        /// \param path Absolute file path of the new rom file
        ///
        ///////////////////////////////////////////////////////////
        bool saveAs(const QString &path);

        ///////////////////////////////////////////////////////////
        /// \brief Seeks to the specified offset within the rom.
        ///
        /// This function fails and will return false in case the
        /// offset to be seeked is outside of the rom's ranges.
        ///
        /// \param offset Location to seek to within the rom
        /// \returns false if the given offset is out of range.
        ///
        ///////////////////////////////////////////////////////////
        bool seek(UInt32 offset) const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the last error that this class threw.
        ///
        /// Returns QString::null if no errors have occured. After
        /// this function, the last error will be destroyed.
        ///
        ///////////////////////////////////////////////////////////
        const QString &lastError() const;


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the currently seeked offset.
        /// \returns the current seeking location.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 offset() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the raw data pointer of this rom.
        ///
        /// The raw data pointer can be used to directly modify
        /// data, but it is not advised to do so. LZ77-related
        /// functions use this in order to speed up operations.
        ///
        /// \returns the raw data pointer of the rom data.
        ///
        ///////////////////////////////////////////////////////////
        UInt8 *data() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves various information about this rom.
        ///
        /// The rom information structure can be used to determine
        /// the rom path, title, code, type and size. It's member
        /// qboy::RomInfo::isLoaded is also useful when creating
        /// a qboy::Rom object on the stack.
        ///
        /// \returns a reference to the current qboy::RomInfo.
        ///
        ///////////////////////////////////////////////////////////
        const RomInfo &info() const;


        ///////////////////////////////////////////////////////////
        /// \brief Determines whether the current offset is valid.
        ///
        /// If an offset is zero or located outside the rom, it
        /// is considered invalid.
        ///
        /// \returns false if the offset is considered invalid.
        ///
        ///////////////////////////////////////////////////////////
        bool checkCurrentOffset() const;

        ///////////////////////////////////////////////////////////
        /// \brief Determines whether the given offset is valid.
        /// \returns false if the offset is considered invalid.
        ///
        ///////////////////////////////////////////////////////////
        bool checkOffset(UInt32 offset) const;

        ///////////////////////////////////////////////////////////
        /// \brief Can be given amount of bytes be read from rom?
        ///
        /// Determines whether the current offset plus the given
        /// amount of bytes is located outside of the rom. For a
        /// greater readability, the enum values of enum
        /// qboy::ValueType (prefix: VT) might be used.
        ///
        /// \returns false if attempting to read outside rom range.
        ///
        ///////////////////////////////////////////////////////////
        bool canRead(Int32 byteCount) const;

        ///////////////////////////////////////////////////////////
        /// \brief Does the same as qboy::Rom::canRead.
        ///
        /// Performs the same operations as qboy::Rom::canRead. It
        /// is separated to increase readability.
        ///
        /// \returns false if attempting to write outside rom range.
        ///
        ///////////////////////////////////////////////////////////
        bool canWrite(Int32 byteCount) const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the offset of the latest pointer.
        ///
        /// Returns the offset from which was read a pointer
        /// recently.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 redirected() const;


        ///////////////////////////////////////////////////////////
        /// \brief Reads one byte at the current position.
        ///
        /// Advances the stream offset by one. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        ///
        /// \returns an unsigned 8-bit value.
        ///
        ///////////////////////////////////////////////////////////
        UInt8 readByte() const;

        ///////////////////////////////////////////////////////////
        /// \brief Reads one half word at the current position.
        ///
        /// Advances the stream offset by two. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        ///
        /// \returns an unsigned 16-bit value.
        ///
        ///////////////////////////////////////////////////////////
        UInt16 readHWord() const;

        ///////////////////////////////////////////////////////////
        /// \brief Reads one word at the current position.
        ///
        /// Advances the stream offset by four. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        ///
        /// \returns an unsigned 32-bit value.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 readWord() const;

        ///////////////////////////////////////////////////////////
        /// \brief Reads one pointer at the current position.
        ///
        /// Advances the stream offset by four. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        /// Null pointers (four 00's) will be returned as zero.
        ///
        /// \returns the offset of the pointer at current position.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 readPointer() const;

        ///////////////////////////////////////////////////////////
        /// \brief Reads the specified amount of bytes.
        ///
        /// Advances the stream offset by the specified amount.
        /// Out-of-range assertion errors will be thrown, but only
        /// in debug mode.
        ///
        /// \returns a QByteArray containing the bytes.
        ///
        ///////////////////////////////////////////////////////////
        QByteArray readBytes(UInt32 count) const;

        ///////////////////////////////////////////////////////////
        /// \brief Reads a table of half words at current offset.
        ///
        /// Advances the stream offset by the given amount
        /// multiplied by two. Assertion errors will be thrown.
        ///
        /// \param count Amount of hwords to read
        /// \returns a list of half words.
        ///
        ///////////////////////////////////////////////////////////
        QList<UInt16> readHWordTable(Int32 count) const;

        ///////////////////////////////////////////////////////////
        /// \brief Reads a table of words at current offset.
        ///
        /// Advances the stream offset by the given amount
        /// multiplied by four. Assertion errors will be thrown.
        ///
        /// \param count Amount of words to read
        /// \returns a list of words.
        ///
        ///////////////////////////////////////////////////////////
        QList<UInt32> readWordTable(Int32 count) const;

        ///////////////////////////////////////////////////////////
        /// \brief Reads a table of pointers at current offset.
        ///
        /// Advances the stream offset by the given amount
        /// multiplied by four. Assertion errors will be thrown.
        ///
        /// \param count Amount of pointers to read
        /// \returns a list of pointers.
        ///
        ///////////////////////////////////////////////////////////
        QList<UInt32> readPointerTable(Int32 count) const;


        ///////////////////////////////////////////////////////////
        /// \brief Writes one byte to the current position.
        ///
        /// Advances the stream offset by one. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        ///
        /// \param byte Unsigned 8-bit value
        ///
        ///////////////////////////////////////////////////////////
        void writeByte(UInt8 byte);

        ///////////////////////////////////////////////////////////
        /// \brief Writes one half word to the current position.
        ///
        /// Advances the stream offset by two. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        ///
        /// \param hword Unsigned 16-bit value
        ///
        ///////////////////////////////////////////////////////////
        void writeHWord(UInt16 hword);

        ///////////////////////////////////////////////////////////
        /// \brief Writes one word to the current position.
        ///
        /// Advances the stream offset by four. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        ///
        /// \param word Unsigned 32-bit value
        ///
        ///////////////////////////////////////////////////////////
        void writeWord(UInt32 word);

        ///////////////////////////////////////////////////////////
        /// \brief Writes one pointer to the current position.
        ///
        /// Takes the given offset, converts it to a pointer and
        /// writes it to the current stream location. NULL offsets
        /// (four 00's) will be written as four zeroes. Advances the
        /// stream offset by four.
        ///
        /// \param offset Unsigned 32-bit offset value
        ///
        ///////////////////////////////////////////////////////////
        void writePointer(UInt32 offset);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the given bytes to the current position.
        ///
        /// Writes the specified byte sequence to the rom and
        /// advances the stream offset by the amount of bytes that
        /// have been written. Out-of-range assertion errors will
        /// be thrown, but only in debug mode.
        ///
        /// \param bytes QByteArray containing the bytes to write
        ///
        ///////////////////////////////////////////////////////////
        void writeBytes(const QByteArray &bytes);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the given hwords to the current position.
        ///
        /// Writes a table of half words to the rom and advances
        /// the stream offset by the amount of hwords multiplied
        /// by two. Out-of-range assertion errors will be thrown.
        ///
        /// \param hwords Table of half words
        ///
        ///////////////////////////////////////////////////////////
        void writeHWordTable(const QList<UInt16> &hwords);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the given words to the current position.
        ///
        /// Writes a table of words to the rom and advances the
        /// stream offset by the amount of hwords multiplied by
        /// four. Out-of-range assertion errors will be thrown.
        ///
        /// \param words Table of words
        ///
        ///////////////////////////////////////////////////////////
        void writeWordTable(const QList<UInt32> &words);

        ///////////////////////////////////////////////////////////
        /// \brief Writes the given pointers to the current position.
        ///
        /// Converts all the offsets in the list to pointers and
        /// writes them to the rom. Advances the stream offset by
        /// the amount of pointers multiplied by four. Out-of-range
        /// assertion errors will be thrown, but only in debug mode.
        ///
        /// \param offsets Table of offsets (not pointers!)
        ///
        ///////////////////////////////////////////////////////////
        void writePointerTable(const QList<UInt32> &offsets);


        ///////////////////////////////////////////////////////////
        /// \brief Expands the rom from 16MB to 32MB.
        ///
        /// This function will absolutely do nothing in case the
        /// rom is already expanded to 32MB.
        ///
        ///////////////////////////////////////////////////////////
        void expand32MB();

        ///////////////////////////////////////////////////////////
        /// \brief Aligns the given offset as specified.
        ///
        /// Alignment can be useful when writing tables or pointers
        /// to the rom which require a specific alignment.
        ///
        /// \param offset Offset to align
        /// \param value Alignment value, e.g. 2
        /// \returns the aligned offset
        ///
        ///////////////////////////////////////////////////////////
        UInt32 alignOffset(UInt32 offset, Int32 value);

        ///////////////////////////////////////////////////////////
        /// \brief Finds some free space within the rom.
        /// \param start Offset to search space from
        /// \param count Amount of bytes to search for
        /// \param byte Byte value to search for (def: 0xFF)
        /// \returns the free space offset (0x0: invalid)
        ///
        ///////////////////////////////////////////////////////////
        UInt32 findSpace(UInt32 start, Int32 count, UInt8 byte = 0xFF);

        ///////////////////////////////////////////////////////////
        /// \brief Finds all occurrences of the specified bytes.
        ///
        /// Starting at the given offset, finds the sequence. Will
        /// return an empty list if sequence not existing.
        ///
        /// \param offset Offset to search sequence from
        /// \param array Byte array sequence to find
        ///
        ///////////////////////////////////////////////////////////
        //QList<UInt32> findBytes(UInt32 offset, const QByteArray &array);


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        RomInfo                 m_Info;
        QByteArray              m_Reference;
        UInt8                  *m_Array;
        UInt32                  m_Length;
        mutable UInt32          m_Offset;
        mutable UInt32          m_Redirected;
        QString                 m_Error;
    };
}


#endif  // __QBOY_ROM_HPP__
