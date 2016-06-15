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
#include <QBoy/Core/Rom.hpp>
#include <QBoy/Core/RomErrors.hpp>
#include <QFile>
#include <cstring>


namespace qboy
{
    ///////////////////////////////////////////////////////////
    // Constructor
    //
    ///////////////////////////////////////////////////////////
    Rom::Rom()
        : m_Array(NULL),
          m_Length(0U),
          m_Offset(0U),
          m_Error(QString::null)
    {
    }


    ///////////////////////////////////////////////////////////
    // Destructor
    //
    ///////////////////////////////////////////////////////////
    Rom::~Rom()
    {
        // Clears the rom data, if not already
        if (!m_Reference.isEmpty())
            m_Reference.clear();

        // Resets all other members
        m_Array = NULL;
        m_Length = 0U;
        m_Offset = 0U;
        m_Error = QString::null;
    }


    ///////////////////////////////////////////////////////////
    // Member I/O functions
    //
    ///////////////////////////////////////////////////////////
    bool Rom::loadFromFile(const QString &path)
    {
        // Determines whether file exists
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
        {
            m_Error = convertFileError(ROM_ERROR_FNF, path);
            return false;
        }

        // Determines whether the file can be read
        m_Reference = file.readAll();
        if (m_Reference.isEmpty() || m_Reference.isNull())
        {
            m_Error = convertFileError(ROM_ERROR_IO, path);
            return false;
        }

        // Destroys the current file handle
        file.close();


        // Makes sure that the rom has the correct size
        if (m_Reference.size() != 16777216 && m_Reference.size() != 33554432)
        {
            m_Error = ROM_ERROR_SIZE;
            return false;
        }

        // Copies necessary values into the class members
        m_Length = static_cast<UInt32>(m_Reference.size());
        m_Array  = reinterpret_cast<UInt8*>(m_Reference.data());

        // Retrieves the rom title and version (16-byte-string)
        m_Offset = 0xA0; // offset of the identifier
        m_Info.setCode(QString(readBytes(16)));

        // Specifies some information about the rom
        m_Info.setExpanded(m_Reference.size() == 33554432);
        m_Info.setPath(path);
        m_Info.setValid(true);
        m_Info.setLoaded(true);


        return true;
    }

    ///////////////////////////////////////////////////////////
    void Rom::close()
    {
        // Resets the rom array
        m_Reference.clear();

        // Resets the necessary I/O information
        m_Info.setValid(false);
        m_Info.setLoaded(false);
    }


    ///////////////////////////////////////////////////////////
    bool Rom::save()
    {
        // Determines whether the file can be opened
        QFile file(m_Info.path());
        if (!file.open(QIODevice::WriteOnly))
        {
            m_Error = file.errorString();
            return false;
        }


        // Writes all done changes to rom
        file.seek(0LL);
        file.write(reinterpret_cast<const Int8 *>(m_Array), static_cast<Int64>(m_Length));
        file.close();
        return true;
    }

    ///////////////////////////////////////////////////////////
    bool Rom::saveAs(const QString &path)
    {
        m_Info.setPath(path);
        return save();
    }

    ///////////////////////////////////////////////////////////
    bool Rom::seek(UInt32 offset) const
    {
        if (offset > m_Length-1)
            return false;

        m_Offset = offset;
        return true;
    }

    ///////////////////////////////////////////////////////////
    const QString &Rom::lastError() const
    {
        return m_Error;
    }


    ///////////////////////////////////////////////////////////
    UInt32 Rom::offset() const
    {
        return m_Offset;
    }

    ///////////////////////////////////////////////////////////
    UInt8 *Rom::data() const
    {
        return m_Array;
    }

    ///////////////////////////////////////////////////////////
    const RomInfo &Rom::info() const
    {
        return m_Info;
    }


    ///////////////////////////////////////////////////////////
    bool Rom::checkCurrentOffset() const
    {
        return (m_Offset < m_Length);
    }

    ///////////////////////////////////////////////////////////
    bool Rom::checkOffset(UInt32 offset) const
    {
        return (offset < m_Length);
    }

    ///////////////////////////////////////////////////////////
    bool Rom::canRead(Int32 byteCount) const
    {
        return (m_Offset + byteCount < m_Length);
    }

    ///////////////////////////////////////////////////////////
    bool Rom::canWrite(Int32 byteCount) const
    {
        return canRead(byteCount);
    }

    ///////////////////////////////////////////////////////////
    UInt32 Rom::redirected() const
    {
        // FIRST has the following purpose:
        // No reversal is needed.
        // Reading ptr1
        // Reading ptr2
        // Loading ptr1 instead of ptr2
        // Loading ptr2 instead of ptr1
        UInt32 temp = m_Redirected.first();
        m_Redirected.removeFirst();

        // Returns the topmost offset
        return temp;
    }

    ///////////////////////////////////////////////////////////
    void Rom::clearCache() const
    {
        m_Redirected.clear();
    }


    ///////////////////////////////////////////////////////////
    // Member read/write functions
    //
    ///////////////////////////////////////////////////////////
    UInt8 Rom::readByte() const
    {
        Q_ASSERT(canRead(VT_Byte));
        return m_Array[m_Offset++];
    }

    ///////////////////////////////////////////////////////////
    UInt16 Rom::readHWord() const
    {
        Q_ASSERT(canRead(VT_HWord));
        UInt8 lo = m_Array[m_Offset++];
        UInt8 hi = m_Array[m_Offset++];

        return (UInt16)((hi << 8) | lo);
    }

    ///////////////////////////////////////////////////////////
    UInt32 Rom::readWord() const
    {
        Q_ASSERT(canRead(VT_Word));
        UInt8 b00 = m_Array[m_Offset++];
        UInt8 b08 = m_Array[m_Offset++];
        UInt8 b16 = m_Array[m_Offset++];
        UInt8 b24 = m_Array[m_Offset++];

        return (UInt32)((b24 << 24) | (b16 << 16) | (b08 << 8) | b00);
    }

    ///////////////////////////////////////////////////////////
    UInt32 Rom::readPointer() const
    {
        // If a NULL pointer is detected, does not substract
        UInt32 pointer = readWord();
        if (pointer == 0x00000000)
            return pointer;
        else
            return (pointer - 0x08000000);
    }

    ///////////////////////////////////////////////////////////
    UInt32 Rom::readPointerRef() const
    {
        m_Redirected.push_back(m_Offset);
        return readPointer();
    }

    ///////////////////////////////////////////////////////////
    QByteArray Rom::readBytes(UInt32 count) const
    {
        Q_ASSERT(canRead(count));

        // Retrieves the offset of the data in the RAM, at current offset
        const char *data = reinterpret_cast<const char *>(m_Array + m_Offset);
        m_Offset += count;

        // Constructs a byte array of data pointer and size
        return { data, static_cast<int>(count) };
    }

    ///////////////////////////////////////////////////////////
    QList<UInt16> Rom::readHWordTable(Int32 count) const
    {
        QList<UInt16> hwords;
        for (int i = 0; i < count; i++)
            hwords.push_back(readHWord());

        return hwords;
    }

    ///////////////////////////////////////////////////////////
    QList<UInt32> Rom::readWordTable(Int32 count) const
    {
        QList<UInt32> words;
        for (int i = 0; i < count; i++)
            words.push_back(readWord());

        return words;
    }

    ///////////////////////////////////////////////////////////
    QList<UInt32> Rom::readPointerTable(Int32 count) const
    {
        QList<UInt32> pointers;
        for (int i = 0; i < count; i++)
            pointers.push_back(readPointer());

        return pointers;
    }


    ///////////////////////////////////////////////////////////
    void Rom::writeByte(UInt8 byte)
    {
        Q_ASSERT(canWrite(VT_Byte));
        m_Array[m_Offset++] = byte;
    }

    ///////////////////////////////////////////////////////////
    void Rom::writeHWord(UInt16 hword)
    {
        Q_ASSERT(canWrite(VT_HWord));
        m_Array[m_Offset++] = (UInt8)(hword & 0xFF);
        m_Array[m_Offset++] = (UInt8)(hword >> 0x8);
    }

    ///////////////////////////////////////////////////////////
    void Rom::writeWord(UInt32 word)
    {
        Q_ASSERT(canWrite(VT_Word));
        m_Array[m_Offset++] = (UInt8)(word & 0xFF);
        m_Array[m_Offset++] = (UInt8)(word >> 0x08);
        m_Array[m_Offset++] = (UInt8)(word >> 0x10);
        m_Array[m_Offset++] = (UInt8)(word >> 0x18);
    }

    ///////////////////////////////////////////////////////////
    void Rom::writePointer(UInt32 offset)
    {
        // If offset is supposed to be a NULL pointer, does not add
        if (offset == 0x00000000)
            writeWord(offset);
        else
            writeWord(offset + 0x08000000);
    }

    ///////////////////////////////////////////////////////////
    void Rom::writeBytes(const QByteArray &bytes)
    {
        Q_ASSERT(canWrite(bytes.size()));
        std::copy(bytes.data(), bytes.data() + bytes.size(), m_Array);
        m_Offset += bytes.size();
    }

    ///////////////////////////////////////////////////////////
    void Rom::writeHWordTable(const QList<UInt16> &hwords)
    {
        int size = hwords.size();
        for (int i = 0; i < size; i++)
            writeHWord(hwords[i]);
    }

    ///////////////////////////////////////////////////////////
    void Rom::writeWordTable(const QList<UInt32> &words)
    {
        int size = words.size();
        for (int i = 0; i < size; i++)
            writeWord(words[i]);
    }

    ///////////////////////////////////////////////////////////
    void Rom::writePointerTable(const QList<UInt32> &offsets)
    {
        int size = offsets.size();
        for (int i = 0; i < size; i++)
            writePointer(offsets[i]);
    }


    ///////////////////////////////////////////////////////////
    // Member misc functions
    //
    ///////////////////////////////////////////////////////////
    void Rom::expand32MB()
    {
        if (m_Length == 33554432)
            return;


        // Creates a new 16MB appendix for the current rom
        Int8 *appendix = new Int8[16777216];
        std::memset(appendix, 0xFF, 16777216);

        // Appends a 16MB array to the current QByteArray.
        m_Reference.append(appendix, 16777216);
        m_Array = reinterpret_cast<UInt8 *>(m_Reference.data());
        m_Length = 33554432;
    }

    ///////////////////////////////////////////////////////////
    UInt32 Rom::alignOffset(UInt32 offset, Int32 value)
    {
        while ((offset % value) != 0)
            offset++;

        return offset;
    }

    ///////////////////////////////////////////////////////////
    UInt32 Rom::findSpace(UInt32 start, Int32 count, UInt8 byte)
    {
        Int32 processed = 0;
        UInt32 foundOffset = start;
        m_Offset = start;

        // Iterates through the rom and breaks until the desired
        // amount of freespace-bytes have been found. Aborts if
        // the current offset goes beyond the rom's bounds.
        while (m_Offset < m_Length && processed < count)
        {
            if (readByte() != byte)
            {
                processed = 0;
                foundOffset = m_Offset;
            }
            else
            {
                processed++;
            }
        }


        // Determines which condition triggered the loop termination
        if (count != processed)
            return 0x0; // invalid
        else
            return foundOffset;
    }
}
