#include "ssh_plugin.h"
#include <qqml.h>

//local
//#include "generatekeys.h"
#include "securenodes.h"
//#include "sftpupload.h"
//#include "sshremoteprocess.h"
//#include "sftpfilesystemmodel.h"


void SshPlugin::registerTypes(const char *uri)
{
    // @uri QmlSsh
//    qmlRegisterType<GenerateKeys>(uri,1,0,"GenerateKeys");
    qmlRegisterType<SecureNodes>(uri,1,0,"SecureNodes");
//    qmlRegisterType<Sftp>(uri,1,0,"Sftp");
//    qmlRegisterType<QmlSshRemoteProcess>(uri,1,0,"SshRemoteProcess");
//    qmlRegisterType<SftpRemoteFileSystemModel>(uri,1,0,"SftpRemoteFileSystemModel");
}

