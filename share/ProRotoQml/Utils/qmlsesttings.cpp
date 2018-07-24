    #include "qmlsesttings.h"

QmlSesttings::QmlSesttings(QObject *parent) : QObject(parent)
{

}

void QmlSesttings::setIniFile()
{
    qDebug ()  << "NOT YET";
}

//void QmlSesttings::setIniFile()
//{
//    QSettings settings(m_file, QSettings::IniFormat);
//    settings.beginGroup("Plugin");
//    settings.setValue( "Name",m_name );
//    settings.setValue( "Type",m_type);
//    settings.setValue( "Description",m_description);
//    settings.setValue( "Version",m_version);
//    settings.endGroup();
//}
