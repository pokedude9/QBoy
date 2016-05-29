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
    include/Core/Rom.hpp \
    include/Config.hpp \
    include/Core/RomInfo.hpp \
    include/Core/RomErrors.hpp \
    include/Text/String.hpp \
    include/Text/Tables.hpp \
    include/Core/Lz77.hpp \
    include/Graphics/Palette.hpp \
    include/Graphics/Color.hpp \
    include/Graphics/PaletteErrors.hpp \
    include/Graphics/Image.hpp \
    include/OpenGL/IndexedTexture.hpp \
    include/OpenGL/GLErrors.hpp \
    include/Graphics/ImageErrors.hpp

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
