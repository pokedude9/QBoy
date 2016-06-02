#
# QBoy: GameboyAdvance library
# Copyright (C) 2015-2016 Pokedude
# License: General Public License 3.0
#


#
# QMake Settings, 1
#
QT         += core opengl
TARGET      = QBoy
TEMPLATE    = lib
COsNFIG     += c++11
DEFINES    += QBOY_LIBRARY

#
# QMake Settings, 2
#
VERSION = 1.0
QMAKE_TARGET_COMPANY = Pokedude
QMAKE_TARGET_PRODUCT = QBoy
QMAKE_TARGET_DESCRIPTION = Handles GBA ROMs and their data
QMAKE_TARGET_COPYRIGHT = (C) 2015-2016 Pokedude

#
# QMake Settings, 3
#
INCLUDEPATH += include


#
# Header Files
#
HEADERS += \
    include/QBoy/Core/Rom.hpp \
    include/QBoy/Config.hpp \
    include/QBoy/Core/RomInfo.hpp \
    include/QBoy/Core/RomErrors.hpp \
    include/QBoy/Text/String.hpp \
    include/QBoy/Text/Tables.hpp \
    include/QBoy/Core/Lz77.hpp \
    include/QBoy/Graphics/Palette.hpp \
    include/QBoy/Graphics/Color.hpp \
    include/QBoy/Graphics/PaletteErrors.hpp \
    include/QBoy/Graphics/Image.hpp \
    include/QBoy/OpenGL/IndexedTexture.hpp \
    include/QBoy/OpenGL/GLErrors.hpp \
    include/QBoy/Graphics/ImageErrors.hpp

#
# Source Files
#
SOURCES += \
    src/Core/RomInfo.cpp \
    src/Core/Rom.cpp \
    src/Text/Tables.cpp \
    src/Text/String.cpp \
    src/Core/Lz77.cpp \
    src/Graphics/Palette.cpp \
    src/OpenGL/GLErrors.cpp \
    src/OpenGL/IndexedTexture.cpp \
    src/Graphics/Image.cpp


#
# Platform-specific settings
#
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES +=

RESOURCES += \
    resources/QBoy.qrc
