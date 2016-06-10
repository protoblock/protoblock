#include "clienidgenerators.h"
#include <QDebug>
#include <QApplication>
#include <QDateTime>

Q_GLOBAL_STATIC(ClientIdGeneratorsFactory, idGeneratorsFactory)

ClientIdGeneratorsFactory *ClientIdGeneratorsFactory::instance()
{
    return idGeneratorsFactory();
}

ClientIdGeneratorPtr ClientIdGeneratorsFactory::getGenerator(IdStyles::Styles style)
{
    if ( m_Cache.contains( (int)style ) ){
        return m_Cache[ (int)style ];
    }

    ClientIdGeneratorPtr newGenerator;
    switch (style) {
    case IdStyles::Unknown :
        qWarning() << Q_FUNC_INFO << "style == IdStyles::Unknown";
        break;

    case IdStyles::Shadow :
        newGenerator.reset( new ClientIdGeneratorShadow );
        break;

    case IdStyles::Azareus :
        newGenerator.reset( new ClientIdGeneratorBram );
        break;

    case IdStyles::Custom:
        newGenerator.reset( new ClientIdGeneratorCustom );
        break;

    default:
        qWarning() << Q_FUNC_INFO << "return nullptr";
        break;
    }

    return newGenerator;
}


QByteArray ClientIdGeneratorShadow::generate()
{
    QString clientId;
    clientId.reserve(20);
    clientId += 'q';
    auto appVer = qApp->applicationVersion();
    auto versionsList = appVer.split('.', QString::SkipEmptyParts);
    for ( QString& version : versionsList ){
        int versionInt = version.toInt();
        if ( versionInt < 10 )
            clientId += version ;
        else if ( versionInt >= 10 && versionInt <= 35 )
            clientId += QChar( 10 - versionInt + 'A' );
        else if ( versionInt >= 36 && versionInt <= 61 )
            clientId += QChar( 36 - versionInt + 'a' );
        else if ( versionInt == 62 )
            clientId += '.';
        else if ( versionInt == 63 )
            clientId += '-';
        else qWarning() << Q_FUNC_INFO << " large app version";
    }
    clientId.append("---");
    clientId = clientId.leftJustified(9, '-', true );

    uint curTime = QDateTime::currentDateTime().toTime_t();
    clientId += QByteArray::number( curTime );

    return clientId.leftJustified(20, '-', true ).toLatin1();
}


QByteArray ClientIdGeneratorBram::generate()
{
    QString clientId;
    clientId.reserve(20);
    uint startupTime = QDateTime::currentDateTime().toTime_t();
    clientId += "-QT";

    auto appVer = qApp->applicationVersion();
    auto versionsList = appVer.split('.', QString::SkipEmptyParts);
    for ( QString& version : versionsList ){
        clientId += version;
    }
    clientId += '-';
    clientId += QByteArray::number(startupTime, 10);

    return clientId.leftJustified(20, '-', true ).toLatin1();
}


QByteArray ClientIdGeneratorCustom::generate()
{
    QString clientId;
    clientId.reserve(20);

    clientId += "Pruch.Prod";
    clientId += QString::number( QDateTime::currentDateTime().toTime_t() );

    return clientId.leftJustified(20, '-', true ).toLatin1();
}
