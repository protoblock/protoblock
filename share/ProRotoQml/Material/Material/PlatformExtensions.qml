/*
 * QML Material - An application framework implementing Material Design.
 *
 * Copyright (C) 2015-2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import Material 1.0

Object {
    id: platform

    property var platformExtensions

    property color decorationColor: Theme.primaryDarkColor
    property var window: null
    readonly property real multiplier: platformExtensions ? platformExtensions.multiplier : 1

    onDecorationColorChanged: {
        if (platformExtensions && color != "#000000") {
            platformExtensions.decorationColor = decorationColor
        }
    }

    onWindowChanged: {
        if (platformExtensions) {
            platformExtensions.window = window
        }
    }
}
