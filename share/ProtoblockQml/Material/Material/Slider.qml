/*
 * QML Material - An application framework implementing Material Design.
 *
 * Copyright (C) 2014 Jordan Neidlinger <JNeidlinger@gmail.com>
 *               2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.4
import QtQuick.Controls 1.3 as Controls
import ProtoblockQml.Utils 1.0
import Material 1.0
import Material.Styles 1.0 as MaterialStyle
import ProtoblockQml.Theme 1.0

/*!
   \qmltype Slider
   \inqmlmodule Material

   \brief Sliders let users select a value from a continuous or discrete range of
   values by moving the slider thumb.
 */
Controls.Slider {
    id: slider

    /*!
       Set to \c true to enable a floating numeric value label above the slider knob
     */
    property bool numericValueLabel: false

    /*!
       Set to \c true to always show the numeric value label, even when not pressed
     */
    property bool alwaysShowValueLabel: false

    /*!
       Set to \c true if the switch is on a dark background
     */
    property bool darkBackground

    /*!
       The label to display within the value label knob, by default the sliders current value
     */
    property string knobLabel: slider.value.toFixed(0)

    /*!
       The diameter of the value label knob
     */
    property int knobDiameter: ProtoScreen.guToPx(4)

    property color color: darkBackground ? Theme.dark.accentColor
                                         : Theme.light.accentColor

    tickmarksEnabled: false

    implicitHeight: numericValueLabel ? ProtoScreen.guToPx(6.75) : ProtoScreen.guToPx(4)
    implicitWidth: ProtoScreen.guToPx(25)

    style: MaterialStyle.SliderStyle {}
}
