#ifndef QQMLNONCONSTPROPERTYHELPERS
#define QQMLNONCONSTPROPERTYHELPERS

#include <QObject>
#include "QQmlHelpersCommon.h"

#define QML_WRITABLE_NONCONST_PROPERTY(type, name) \
    protected: \
        Q_PROPERTY (type name READ MAKE_GETTER_NAME (name) WRITE set_##name NOTIFY name##Changed) \
    private: \
        type m_##name; \
    public: \
         type & MAKE_GETTER_NAME (name) (void)  { \
            return m_##name ; \
        } \
    public Q_SLOTS: \
        bool set_##name ( type name) { \
            bool ret = false; \
            if ((ret = m_##name != name)) { \
                m_##name = name; \
                emit name##Changed (m_##name); \
            } \
            return ret; \
        } \
    Q_SIGNALS: \
        void name##Changed ( type  name); \
    private:

#define QML_READONLY_NONCONST_PROPERTY(type, name) \
    protected: \
        Q_PROPERTY (type name READ MAKE_GETTER_NAME (name) NOTIFY name##Changed) \
    private: \
        type m_##name; \
    public: \
         type & MAKE_GETTER_NAME (name) (void)  { \
            return m_##name ; \
        } \
        bool set##name (type name) { \
            bool ret = false; \
            if ((ret = m_##name != name)) { \
                m_##name = name; \
                emit name##Changed (m_##name); \
            } \
            return ret; \
        } \
    Q_SIGNALS: \
        void name##Changed ( type name); \
    private:

#define QML_NONCONST_PROPERTY(type, name) \
    protected: \
        Q_PROPERTY (type name READ MAKE_GETTER_NAME (name) ) \
    private: \
        type m_##name; \
    public: \
        type & MAKE_GETTER_NAME (name) (void)  { \
            return m_##name ; \
        } \
    private:

class _QmlNonConstProperty_ : public QObject {
    Q_OBJECT
    QML_WRITABLE_NONCONST_PROPERTY (int,     var1)
    QML_READONLY_NONCONST_PROPERTY (bool,    var2)
    QML_NONCONST_PROPERTY (QString, var3)
};

#endif // QQMLNONCONSTCONSTPROPERTYHELPERS
