#ifndef QQMLENUMCLASS
#define QQMLENUMCLASS

#include <QObject>

#define QML_ENUM_CLASS(name, ...) \
    class name : public QObject { \
        Q_GADGET \
    public: \
        enum Type { __VA_ARGS__ }; \
        Q_ENUMS (Type) \
    };

class _Test_QmlEnumClass_ {
    Q_GADGET
}; // NOTE : to avoid "no suitable class found" MOC note

#endif // QQMLENUMCLASS

