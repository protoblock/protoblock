osx {
#QMAKE_MAC_SDK = macosx10.12
}

TEMPLATE = subdirs
CONFIG += ordered c++11

SUBDIRS += \
       Material \
       Material/ListItems \
       Material/Extras \
       Material/Styles

CONFIG(debug,debug|release):message("QtMaterial Debug mode")
CONFIG(release,debug|release):message("QtMaterial Release mode")
