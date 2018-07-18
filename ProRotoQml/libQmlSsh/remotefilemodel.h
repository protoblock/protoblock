#ifndef REMOTEFILEMODEL_H
#define REMOTEFILEMODEL_H
//#include "sftpdefs.h"
//#include "ssh_global.h"
//#include "sftpfilesystemmodel.h"
#include <QAbstractItemModel>
#include <QObject>

class RemoteFileModel : public QSsh::SftpFileSystemModel
{
    Q_OBJECT
public:
    explicit RemoteFileModel(QSsh::SftpFileSystemModel *parent = 0);

signals:

public slots:

};

#endif // REMOTEFILEMODEL_H
