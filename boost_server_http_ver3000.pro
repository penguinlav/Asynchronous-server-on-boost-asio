TEMPLATE = app
CONFIG += console c++11 c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    http_header.cpp \
    tcp_connection.cpp \
    tcp_server.cpp

#boost
INCLUDEPATH += $$PWD/../../boost/include/boost-1_64
LIBS += -L$$PWD/../../boost/lib -llibboost_filesystem-mgw49-mt-1_64 \
                                -llibboost_container-mgw49-mt-1_64 \
                                -llibboost_iostreams-mgw49-mt-1_64 \
                                -llibboost_system-mgw49-mt-1_64 \
                                -llibboost_locale-mgw49-mt-1_64

LIBS += -lws2_32
LIBS += -lwsock32

HEADERS += \
    http_header.h \
    tcp_connection.h \
    tcp_server.h
