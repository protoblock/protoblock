#include "files_plugin.h"

#include "qmlfileinfo.h"
#include "qmlfilesystemwatcher.h"
#include "qfile.h"
#include "qmldir.h"
#include  "qmlmimetype.h"

#include <qqml.h>

void ProRotoQmlFilesPlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Files
    qmlRegisterType<QmlFile>(uri,1,0,"QmlFile");
    qmlRegisterType<QmlDir>(uri,1,0,"QmlDir");
    qmlRegisterType<QmlMimeType>(uri,1,0,"QmlMimeType");
    qmlRegisterType<QmlFileInfo>(uri,1,0,"QmlFileInfo");
    qmlRegisterType<QmlFileSystemWatcher>(uri,1, 0, "QmlFileSystemWatcher");
}

