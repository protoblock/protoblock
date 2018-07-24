#ifndef CONTEXTPROPERTYCHANGELISTENER_H
#define CONTEXTPROPERTYCHANGELISTENER_H

#include <QObject>

class QQmlContext;

class ContextPropertyChangeListener : public QObject
{
    Q_OBJECT
public:
    explicit ContextPropertyChangeListener(QQmlContext* context, const QString& contextProperty);
    Q_SLOT void updateContextProperty();
    QQmlContext* m_context;
    QString m_contextProperty;
};
#endif // CONTEXTPROPERTYCHANGELISTENER_H
