#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = 0);


    Q_INVOKABLE QString formatSize(const int size);

signals:

public slots:
};

#endif // UTILS_H
