QT -= gui
QT += network httpserver

CONFIG += c++17 console
CONFIG -= app_bundle

QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.5

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L"/opt/homebrew/Cellar/openssl@1.1/1.1.1q/lib" -lcrypto -lssl

SOURCES += \
        action.cpp \
        game.cpp \
        main.cpp \
        game_engine.cpp \
        game_session.cpp \
        messages.cpp \
        player.cpp \
        rank.cpp \
        paths.cpp \
        thread.cpp \
        uuid.cpp

HEADERS += \
        game_engine.hpp \
        game.hpp \
        game_id.hpp \
        game_session.hpp \
        player.hpp \
        action.hpp \
        inactive_reciver.hpp \
        inactive_detection.hpp \
        interface_id.hpp \
        messages.hpp \
        player_id.hpp \
        rank.hpp \
        paths.hpp \
        thread.hpp \
        uuid.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

