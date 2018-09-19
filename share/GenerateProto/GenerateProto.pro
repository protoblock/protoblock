include($$PWD/genproto.pri)
TARGET = ProtoblockProtocols
TEMPLATE = lib
QT -= core gui

CONFIG += \
    c++11

QMAKE_DISTCLEAN += -r $$GENERATEDDIR

HEADERS += \
    $$GENERATEDDIR/P2PData.pb.h \
    $$GENERATEDDIR/ExData.pb.h \
    $$GENERATEDDIR/NameData.pb.h \
    $$GENERATEDDIR/StaticData.pb.h \
    $$GENERATEDDIR/StatusData.pb.h\
    $$GENERATEDDIR/ProtoData.pb.h \
    $$GENERATEDDIR/StateData.pb.h \
    $$GENERATEDDIR/ApiData.pb.h

headers.files += $$HEADERS
headers.CONFIG += no_check_exist

linux:!macx {
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_DESCRIPTION = Protocols for protoblock
    QMAKE_PKGCONFIG_NAME = protoblock-protocols

    !isEmpty(INSTALL_PREFIX){
        headers.path = $$INSTALL_PREFIX/include/protoblock-protocols
        target.path = $$INSTALL_PREFIX/lib
        QMAKE_PKGCONFIG_PREFIX = $$INSTALL_PREFIX
    }

    isEmpty(INSTALL_PREFIX){
        headers.path = /usr/include/protoblock-protocols
        target.path = /usr/lib
        QMAKE_PKGCONFIG_PREFIX = /usr
    }

    QMAKE_PKGCONFIG_LIBDIR = $$target.path
    QMAKE_PKGCONFIG_INCDIR = $$headers.path
    QMAKE_PKGCONFIG_VERSION = $$VERSION
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}


win32|win64{
    CONFIG += static
    headers.path = $$INSTALL_PREFIX/protoblock-protocols/usr/include
    target.path = $$INSTALL_PREFIX/protoblock-protocols/usr/lib
}
ios{
    headers.path = $$INSTALL_PREFIX/usr/local/include/protoblock-protocols
    target.path = $$INSTALL_PREFIX/usr/local/lib
}
macx{
    headers.path = $$INSTALL_PREFIX/usr/local/include/protoblock-protocols
    target.path = $$INSTALL_PREFIX/usr/local/lib
}

INSTALLS += \
    target \
    headers
