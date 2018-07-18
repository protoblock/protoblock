
#include "palette.h"


Palette::Palette(QObject *parent)
    : QObject(parent),
      m_file("NULL")

{
    setDefaultIniFile();
}

QString Palette::window() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("Window").toString();
    settings.endGroup();
    return win;
}

void Palette::setWindowColor(const QString color)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("Window",color);
    settings.endGroup();
}

QString Palette::windowText() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("WindowText").toString();
    settings.endGroup();
    return win;
}

void Palette::setWindowText(const QString windowText)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("WindowText",windowText);
    settings.endGroup();
}

QString Palette::base() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("Base").toString();
    settings.endGroup();
    return win;
}

void Palette::setBaseColor(const QString base)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("Base", base);
    settings.endGroup();
}

QString Palette::colorTheme() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("ColorTheme").toString();
    settings.endGroup();
    return win;
}

void Palette::setColorTheme(const QString colorTheme)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("ColorTheme", colorTheme);
    settings.endGroup();
}

QString Palette::buttonText() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("ButtonText").toString();
    settings.endGroup();
    return win;
}

void Palette::setButtonText(const QString buttonText)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("ButtonText", buttonText);
    settings.endGroup();
}

QString Palette::button() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("Button").toString();
    settings.endGroup();
    return win;
}

void Palette::setButton(const QString button)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("Button", button);
    settings.endGroup();
}

QString Palette::text() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("Text").toString();
    settings.endGroup();
    return win;
}

void Palette::setText(const QString text)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("Text", text);
    settings.endGroup();
}

QString Palette::alternateBase() const
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString win = settings.value("AlternateBase").toString();
    settings.endGroup();
    return win;
}

void Palette::alternateBase(const QString alternateBase)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    settings.setValue("AlternateBase", alternateBase);
    settings.endGroup();
}

QString Palette::getPaletteValue(const QString &val)
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Palette");
    QString value = settings.value(val).toString();
    settings.endGroup();
    return value ;
}

QString Palette::file() const
{
    return m_file;
}

void Palette::setFile(const QString file)
{
    if (m_file == file)
        return;
    m_file = file;
    emit fileChanged();
    emit paletteChanged();
    fooChanged();
}

void Palette::setDefaultIniFile()
{
    QString theTheme;
    QString defaultFile = QString("%1/%2").arg(QGuiApplication::applicationDirPath()).arg("Palette.ini");
    setFile(defaultFile);
    m_yColors.init();

    QFile settingsFile(defaultFile);
    if( ! settingsFile.exists() ){
        //set the blue to be darker
        QColor m_color( m_yColors.blue() );
        //FIXME make into hex
        m_color.darker(200);
        QSettings settings(m_file, QSettings::IniFormat);
        settings.beginGroup("Palette");
                settings.setValue( "Window",m_yColors.cyan() );
                settings.setValue( "WindowText" , m_yColors.black() );
                settings.setValue( "Base", m_yColors.blue() );
                settings.setValue( "Text",m_yColors.black() );
                settings.setValue( "AlternateBase" , m_color);
                settings.setValue( "Button", m_yColors.grey() );
                switch ( m_yColors.theme() ) {
                case Colors::Material:
                    theTheme = "Material";
                    break;
                case Colors::Ubuntu:
                    theTheme = "Ubuntu";
                    break;
                case Colors::Flat:
                    theTheme = "Flat";
                    break;
                case Colors::Metro:
                    theTheme = "Metro";
                    break;
                case Colors::Mappel:
                    theTheme = "Mappel";
                    break;
                }
                settings.setValue("ColorTheme",theTheme);
        settings.endGroup();
    }
}


