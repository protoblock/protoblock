#include "sftpfilemodel.h"

SftpFileInfo::SftpFileInfo(const QString &name, const int &inType, const int &size )
    : m_name(name),
      m_inType(inType),
      m_size(size)
{
}

QString SftpFileInfo::name() const
{
    return m_name;
}

QString SftpFileInfo::type() const
{


    switch (m_inType) {
    case 0:
        m_type = "File";
        break;
    case 1:
        m_type = "Dir";
        break;
    case 2:
        m_type = "Other";
        break;
    case 3 :
        m_type = "Unknown";
        break;
    }
    return m_type;
}


int SftpFileInfo::size() const
{
    return m_size;
}

SftpFileModel::SftpFileModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void SftpFileModel::addSftpFile(const SftpFileInfo &fileInfo)
{
    // clear the model
    m_roles.clear();
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_roles << fileInfo;
    endInsertRows();
}


int SftpFileModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_roles.count();
}

//void SftpFileModel::SftpFileModel::clearModel()
//{
//    qDebug() << "clear the model";
//}

QVariant SftpFileModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_roles.count())
        return QVariant();
    const SftpFileInfo &roleName  = m_roles[index.row()];
    if (role == NameRole)
        return roleName.name();
    else if (role == TypeRole)
        return roleName.type();
    else if (role == SizeRole)
        return roleName.size();
    return QVariant();
}

QHash<int, QByteArray> SftpFileModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    return roles;
}
//![0]

