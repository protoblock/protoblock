#############################################################################
# Makefile for building: ProRotoQml.Models.app/Contents/MacOS/ProRotoQml.Models
# Generated by qmake (3.0) (Qt 5.6.0)
# Project:  models.pro
# Template: app
# Command: /Users/satoshi/Qt/5.6/ios/bin/qmake -spec macx-ios-clang CONFIG+=release CONFIG+=iphoneos CONFIG+=device -spec macx-xcode -o ProRotoQml.Models.xcodeproj/project.pbxproj models.pro
#############################################################################

MAKEFILE      = project.pbxproj

MOC       = /Users/satoshi/Qt/5.6/ios/bin/moc
UIC       = 
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DDARWIN_NO_CARBON -DQT_NO_PRINTER -DQT_NO_PRINTDIALOG -DQT_NO_DEBUG -DQT_STATICPLUGIN -DQT_PLUGIN -DQT_QUICK_LIB -DQT_GUI_LIB -DQT_QML_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB
INCPATH       = -I. -I/Users/satoshi/Qt/5.6/ios/mkspecs/macx-ios-clang/ios -I/Users/satoshi/Qt/5.6/ios/include -I/Users/satoshi/Qt/5.6/ios/include/QtQuick -I/Users/satoshi/Qt/5.6/ios/include/QtGui -I/Users/satoshi/Qt/5.6/ios/include/QtQml -I/Users/satoshi/Qt/5.6/ios/include/QtNetwork -I/Users/satoshi/Qt/5.6/ios/include/QtCore -I. -I/Users/satoshi/Qt/5.6/ios/mkspecs/macx-ios-clang
DEL_FILE  = rm -f
MOVE      = mv -f

preprocess: compilers
clean preprocess_clean: compiler_clean

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compilers: moc_models_plugin.cpp moc_sortfilterproxymodel.cpp
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all: moc_models_plugin.cpp moc_sortfilterproxymodel.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_models_plugin.cpp moc_sortfilterproxymodel.cpp
moc_models_plugin.cpp: /Users/satoshi/Qt/5.6/ios/include/QtQml/QQmlExtensionPlugin \
		models_plugin.h
	/Users/satoshi/Qt/5.6/ios/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -D__APPLE_CC__ -I/Users/satoshi/Qt/5.6/ios/mkspecs/macx-ios-clang -I/Users/satoshi/Desktop/fc/osx/ProRoto2016/ProRotoQml/Models -I/Users/satoshi/Qt/5.6/ios/mkspecs/macx-ios-clang/ios -I/Users/satoshi/Qt/5.6/ios/include -I/Users/satoshi/Qt/5.6/ios/include/QtQuick -I/Users/satoshi/Qt/5.6/ios/include/QtGui -I/Users/satoshi/Qt/5.6/ios/include/QtQml -I/Users/satoshi/Qt/5.6/ios/include/QtNetwork -I/Users/satoshi/Qt/5.6/ios/include/QtCore -Muri=ProRotoQml.Models models_plugin.h -o moc_models_plugin.cpp

moc_sortfilterproxymodel.cpp: /Users/satoshi/Qt/5.6/ios/include/QtCore/qsortfilterproxymodel.h \
		/Users/satoshi/Qt/5.6/ios/include/QtQml/qqmlparserstatus.h \
		/Users/satoshi/Qt/5.6/ios/include/QtQml/qjsvalue.h \
		/Users/satoshi/Qt/5.6/ios/include/QtCore/QDebug \
		sortfilterproxymodel.h
	/Users/satoshi/Qt/5.6/ios/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -D__APPLE_CC__ -I/Users/satoshi/Qt/5.6/ios/mkspecs/macx-ios-clang -I/Users/satoshi/Desktop/fc/osx/ProRoto2016/ProRotoQml/Models -I/Users/satoshi/Qt/5.6/ios/mkspecs/macx-ios-clang/ios -I/Users/satoshi/Qt/5.6/ios/include -I/Users/satoshi/Qt/5.6/ios/include/QtQuick -I/Users/satoshi/Qt/5.6/ios/include/QtGui -I/Users/satoshi/Qt/5.6/ios/include/QtQml -I/Users/satoshi/Qt/5.6/ios/include/QtNetwork -I/Users/satoshi/Qt/5.6/ios/include/QtCore -Muri=ProRotoQml.Models sortfilterproxymodel.h -o moc_sortfilterproxymodel.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

