#ifndef QMLSQLTABLEMODEL_H
#define QMLSQLTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlError>


#include <QDebug>
class QmlSqlTableModel : public QSqlTableModel{
    Q_OBJECT
    Q_PROPERTY(QString table READ table WRITE setT NOTIFY tableNameChanged)
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)

    Q_PROPERTY(SortType sortType READ sortType WRITE setSortType NOTIFY sortTypeChanged)
    Q_ENUMS(SortType)
    Q_PROPERTY(EditStrategy editStrategy READ editStrategy WRITE setEditStrategy NOTIFY editStrategyChanged)
    Q_ENUMS(EditStrategy)

public:
    explicit QmlSqlTableModel(QObject *parent = 0, QSqlDatabase db);



    enum EditStrategy { FieldChange, RowChange, ManualSubmit };
    EditStrategy  editStrategy() const;
    void setEditStrategy(const EditStrategy &editStrategy);


    enum SortType{ Ascending, Descending };
    SortType sortType()const;
    void setSortType(const SortType sortType);

    QString connectionName()const;
    void setConnectionName(const QString &connectionName);

    QString table()const;
    void setT(const QString &table);

    Q_INVOKABLE void init();
    QVariant data(const QModelIndex &index, int role=Qt::EditRole ) const;
    QHash<int, QByteArray>roleNames() const;
    QStringList rolesList() const;
    Q_INVOKABLE int count()const;

signals:
    void tableNameChanged();
    void connectionNameChanged();
    void countChanged();
    void sortTypeChanged();
    void editStrategyChanged();

public slots:

private:
    QSqlTableModel* d;

    QString m_tableName;
    QString m_connectionName;
    int m_count;

    SortType m_sortType;
    EditStrategy m_editStrategy;
    QSqlTableModel::EditStrategy mm_editStrategy;

    QStringList m_roleList;
    QHash<int, QByteArray> m_roleNames;
};

#endif // QMLSQLTABLEMODEL_H
