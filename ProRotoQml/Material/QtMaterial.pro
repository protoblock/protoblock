osx {
QMAKE_MAC_SDK = macosx10.12
}

TEMPLATE = subdirs
CONFIG += ordered \
                     c++11

SUBDIRS += \
                       Material \
                       Material/ListItems \
                       Material/Extras \
                       Material/Styles
