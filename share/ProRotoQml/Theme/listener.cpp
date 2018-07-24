#include "listener.h"
#include <QQmlContext>

ContextPropertyChangeListener::ContextPropertyChangeListener(QQmlContext *context, const QString &contextProperty) :
    QObject(context),
    m_context(context),
    m_contextProperty(contextProperty)
{
}

void ContextPropertyChangeListener::updateContextProperty()
{
    QVariant value = m_context->contextProperty(m_contextProperty);
    m_context->setContextProperty(m_contextProperty, value);
}
