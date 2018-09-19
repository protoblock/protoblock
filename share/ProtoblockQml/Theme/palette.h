#ifndef PALETTE_H
#define PALETTE_H

#include <QObject>
#include <QPalette>
#include "colors.h"
#include <QSettings>
#include <QDebug>
#include <QGuiApplication>
#include <QFile>

class  Palette : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file NOTIFY fileChanged)

    Q_PROPERTY(QString window READ window NOTIFY paletteChanged)
    Q_PROPERTY(QString windowText READ windowText NOTIFY paletteChanged)
    Q_PROPERTY(QString base READ base NOTIFY paletteChanged)
    Q_PROPERTY(QString text READ text NOTIFY paletteChanged)
    Q_PROPERTY(QString alternateBase READ alternateBase NOTIFY paletteChanged)
    Q_PROPERTY(QString button READ button NOTIFY paletteChanged)
    Q_PROPERTY(QString buttonText READ buttonText NOTIFY paletteChanged)
    Q_PROPERTY(QString colorTheme READ colorTheme NOTIFY paletteChanged)

public:
    Palette(QObject *parent=0);

    QString window() const;
    Q_INVOKABLE void setWindowColor(const QString color);

    QString windowText() const;
    Q_INVOKABLE void setWindowText(const QString windowText);

    QString base() const;
    Q_INVOKABLE void setBaseColor(const QString base);

    QString colorTheme() const;
    Q_INVOKABLE void setColorTheme(const QString colorTheme);

    QString buttonText() const;
    Q_INVOKABLE void setButtonText(const QString buttonText);

    QString button() const;
    Q_INVOKABLE void setButton(const QString button);

    QString text() const;
    Q_INVOKABLE void setText(const QString text);


    QString alternateBase() const;
    Q_INVOKABLE void alternateBase(const QString alternateBase);


    Q_INVOKABLE QString getPaletteValue(const QString &val);
    void setDefaultIniFile();

   Q_INVOKABLE void  setFile(const QString file);
    QString file() const;

signals:
    void paletteChanged();
    void fileChanged();
    void fooChanged();
private:
    Colors m_yColors;
    QString m_file;
    QColor m_window;
};



#endif // PALETTE_H
