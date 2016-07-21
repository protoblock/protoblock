#ifndef DEFS_H
#define DEFS_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QDate>

#include "../QmlSuperMacros/QQmlVarPropertyHelpers.h"
#include "../QmlSupermacros/QQmlConstRefPropertyHelpers.h"
#include "../QmlSuperMacros/QQmlAutoPropertyHelpers.h"

class TestModelItem : public QObject {
    Q_OBJECT
    QML_CONSTANT_VAR_PROPERTY    (int,     day)
    QML_CONSTANT_CSTREF_PROPERTY (QString, month)
    QML_CONSTANT_VAR_PROPERTY    (int,     year)
    QML_WRITABLE_VAR_PROPERTY    (bool,    selected)
    QML_READONLY_AUTO_PROPERTY   (QString, str)

public:
    explicit TestModelItem (const QDateTime & when, QObject * parent = Q_NULLPTR);
    void setModelUp();
};

#endif // DEFS_H
