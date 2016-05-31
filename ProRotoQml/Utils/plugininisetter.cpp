#include "plugininisetter.h"

PluginIniSetter::PluginIniSetter(QObject *parent) : QObject(parent)
{
}

QString PluginIniSetter::type() const
{
    return m_type;
}

void PluginIniSetter::setType(const QString &type)
{
    if (m_type == type)
        return;
    m_type = type ;
    emit typeChanged();
}

QString PluginIniSetter::name() const
{
    return m_name;
}

void PluginIniSetter::setName(const QString &name)
{
    if (m_name == name)
        return;
    m_name = name;
    emit nameChanged();
}

QString PluginIniSetter::version() const
{
    return m_version;
}

void PluginIniSetter::setVersion(const QString &version)
{
    if (m_version == version)
        return;
    m_version = version;
    emit versionChanged();
}

QString PluginIniSetter::description() const
{
    return m_description ;
}

void PluginIniSetter::setDescription(const QString &description)
{
    if (m_description == description)
        return;
    m_description = description;
    emit descriptionChanged();
}

QString PluginIniSetter::file() const
{
    return m_file;
}

void PluginIniSetter::setFile(const QString &file)
{
    if (m_file == file)
        return;
    m_file = file;
    emit fileChanged();
}

void PluginIniSetter::setIniFile()
{
    QSettings settings(m_file, QSettings::IniFormat);
    settings.beginGroup("Plugin");
        settings.setValue( "Name",m_name );
        settings.setValue( "Type",m_type);
        settings.setValue( "Description",m_description);
        settings.setValue( "Version",m_version);
    settings.endGroup();
}
