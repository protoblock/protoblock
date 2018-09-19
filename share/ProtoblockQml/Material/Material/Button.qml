/*
 * QML Material - An application framework implementing Material Design.
 *
 * Copyright (C) 2014-2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.4
import QtQuick.Controls 1.4 as Controls
import Material.Styles 1.0 as MaterialStyle
import Material 1.0
//import ProtoblockQml.Theme 1.0

Controls.Button {
    id: button

//    height: ProtoScreen.guToPx(4)

    /*!
       The background color of the button. By default, this is white for a raised
       button and transparent for a flat button.
     */
    property color backgroundColor: elevation > 0 ? "white" : "transparent"

    /*!
       \internal

       The context of the button, which is used to control special styling of
       buttons in dialogs or snackbars.
     */
    property string context: "default" // or "dialog" or "snackbar"

    /*!
       Set to \c true if the button has a dark background color
     */
    property bool darkButton: Theme.isDarkColor(backgroundColor)

    /*!
       Set to \c true if the button is on a dark background
     */
    property bool darkBackground

    /*!
       The elevation of the button. Normally either \c 0 or \c 1.
     */
    property int elevation

    /*!
       The text color of the button. By default, this is calculated based on the background color,
       but it can be customized to the theme's primary color or another color.
     */
    property color textColor: button.darkButton ? Theme.dark.textColor : Theme.light.textColor

    style: MaterialStyle.ButtonStyle {}
}
