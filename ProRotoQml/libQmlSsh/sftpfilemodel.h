#include <QAbstractListModel>
#include <QStringList>
#include <QString>

class SftpFileInfo
{

public:
    SftpFileInfo(const QString &name, const int &inType, const int &size );
    QString name() const;
    int size() const;
    QString type()const;

private:
    QString m_name;
    int m_size;

    QString m_type;
    int m_inType;

};

class SftpFileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FileRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        SizeRole
    };

    SftpFileModel(QObject *parent = 0);
    void addSftpFile(const SftpFileInfo &fileInfo);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<FileRoles> m_roles;
};


