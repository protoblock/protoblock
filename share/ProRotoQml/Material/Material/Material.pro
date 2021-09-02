osx {
#QMAKE_MAC_SDK = macosx10.12
}
TEMPLATE = lib
TARGET = Material
QT += qml quick core
CONFIG += qt plugin c++11
win32:{
    CONFIG+= static
}

TARGET = $$qtLibraryTarget($$TARGET)
uri = Material



DISTFILES = qmldir \
    awesome.js \
    utils.js \
    Action.qml \
    ActionBar.qml \
    ActionButton.qml \
    ApplicationWindow.qml \
    AppTheme.qml \
    AwesomeIcon.qml \
    BottomActionSheet.qml \
    BottomSheet.qml \
    Button.qml \
    Card.qml \
    CheckBox.qml \
    ComboBox.qml \
    DatePicker.qml \
    Dialog.qml \
    Dropdown.qml \
    Icon.qml \
    IconButton.qml \
    Ink.qml \
    InputDialog.qml \
    Label.qml \
    MainView.qml \
    MaterialAnimation.qml \
    MenuField.qml \
    NavigationDrawer.qml \
    NavigationDrawerPage.qml \
    Object.qml \
    OverlayLayer.qml \
    OverlayView.qml \
    Page.qml \
    PageSidebar.qml \
    PageStack.qml \
    Palette.qml \
    Popover.qml \
    PopupBase.qml \
    ProgressBar.qml \
    ProgressCircle.qml \
    RadioButton.qml \
    Scrollbar.qml \
    Sidebar.qml \
    Slider.qml \
    Snackbar.qml \
    Switch.qml \
    Tab.qml \
    TabBar.qml \
    TabbedPage.qml \
    TextField.qml \
    Theme.qml \
    ThemePalette.qml \
    ThinDivider.qml \
    TimePicker.qml \
    TimePickerDialog.qml \
    Toolbar.qml \
    Tooltip.qml \
    UnitsHelper.qml \
    View.qml \
    Wave.qml \
    Window.qml



!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = $$DISTFILES
icons.files += \
    icons/navigation_arrow_back.svg \
    icons/navigation_chevron_left.svg \
    icons/navigation_chevron_right.svg \
    icons/navigation_close.svg \
    icons/navigation_menu.svg \
    icons/navigation_more_vert.svg \
    icons/core_icons.qrc

fonts.files += \
    fonts/FontAwesome.otf \
    fonts/MaterialFontLoader.qml \
    fonts/fonts.qrc \
    fonts/qmldir

fontsRoboto.files += \
    fonts/roboto/Roboto-Black.ttf \
    fonts/roboto/Roboto-BlackItalic.ttf \
    fonts/roboto/Roboto-Bold.ttf \
    fonts/roboto/Roboto-BoldItalic.ttf \
    fonts/roboto/Roboto-Italic.ttf \
    fonts/roboto/Roboto-Light.ttf \
    fonts/roboto/Roboto-LightItalic.ttf \
    fonts/roboto/Roboto-Medium.ttf \
    fonts/roboto/Roboto-MediumItalic.ttf \
    fonts/roboto/Roboto-Regular.ttf \
    fonts/roboto/Roboto-Thin.ttf \
    fonts/roboto/Roboto-ThinItalic.ttf \
    fonts/roboto/RobotoCondensed-Bold.ttf \
    fonts/roboto/RobotoCondensed-BoldItalic.ttf \
    fonts/roboto/RobotoCondensed-Italic.ttf \
    fonts/roboto/RobotoCondensed-Light.ttf \
    fonts/roboto/RobotoCondensed-LightItalic.ttf \
    fonts/roboto/RobotoCondensed-Regular.ttf

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath

icons.path = $$installPath/icons

fonts.path = $$installPath/fonts/
fontsRoboto.path = $$installPath/fonts/roboto/
INSTALLS += target qmldir icons fonts

HEADERS += \
    dummyios.h

SOURCES += \
    dummyios.cpp

RESOURCES += \
    material.qrc \
    popups.qrc \
    window.qrc \
    components.qrc \
    controls.qrc \
    core.qrc \
    fonts/fonts.qrc

message("materialpro" $$installPath)
CONFIG(debug,debug|release):message("Debug mode")
CONFIG(release,debug|release):message("Release mode")
