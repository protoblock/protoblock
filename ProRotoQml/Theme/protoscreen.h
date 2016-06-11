#ifndef PROTOSCREEN_H
#define PROTOSCREEN_H



#include <QObject>
#include <QtQml>
#include <QObject>
#include <QtQml>
class ProtoDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deviceModelIdentifier READ deviceModelIdentifier NOTIFY dataChanged)

public:
    static QObject *singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static ProtoDevice* instance(QQmlEngine *engine);

    Q_INVOKABLE static QString deviceModelIdentifier() { return m_deviceModelIdentifier; }

signals:
    void dataChanged();

protected:
    explicit ProtoDevice(QObject *parent = 0);

    static QString m_deviceModelIdentifier;
    static ProtoDevice* m_pInstance;
};

class ProtoScreen : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int desktopWidth READ desktopWidth NOTIFY gridUnitChanged )
    Q_PROPERTY( int desktopHeight READ desktopHeight NOTIFY gridUnitChanged )
    Q_PROPERTY( int designWidth READ designWidth NOTIFY gridUnitChanged )
    Q_PROPERTY( int designHeight READ designHeight NOTIFY gridUnitChanged )
    Q_PROPERTY( double displaySize READ displaySize NOTIFY gridUnitChanged )
    Q_PROPERTY( double devicePixelRatio READ devicePixelRatio NOTIFY gridUnitChanged )
    Q_PROPERTY( double gridUnit READ gridUnit NOTIFY gridUnitChanged )

    Q_ENUMS( CuteFont )

public:
    enum CuteFont {
        FONT_NOTSET = 0,
        FONT_XXLARGE,
        FONT_XLARGE,
        FONT_LARGE,
        FONT_MEDIUM,
        FONT_NORMAL,
        FONT_SMALL,
        FONT_TINY
    };

    static QObject *singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static ProtoScreen* instance(QQmlEngine *engine);

    Q_INVOKABLE double guToPx(double units);
    Q_INVOKABLE double pxToGu(double px);

    Q_INVOKABLE void setDesignResolution(int width, int height);

    Q_INVOKABLE double font(CuteFont cutefont) { return m_fonts[cutefont]; }

signals:
    void gridUnitChanged();

protected slots:
    void updateFonts();
    void updateGridUnit();

protected:
    void initialize();
    bool isInitialized() { return m_bInitialized; }

    void setGridUnit(double unit);
    double gridUnit() { return m_gridUnit; }

    int desktopWidth() { return m_desktopGeometry.width(); }
    int desktopHeight() { return m_desktopGeometry.height(); }

    int designWidth() { return m_designResolution.width(); }
    int designHeight() { return m_designResolution.height(); }
    double displaySize() { return m_displayDiagonalSize; }
    double devicePixelRatio() { return m_devicePixelRatio; }

    bool m_bInitialized;

    double m_gridUnit;
    double m_defaultGrid;
    double m_devicePixelRatio;
    double m_dpiX;
    double m_dpiY;
    double m_displayDiagonalSize;

    QRect m_desktopGeometry;
    QRect m_designResolution;

    QMap<CuteFont, double> m_fonts;

    explicit ProtoScreen(QObject *parent = 0);
    static ProtoScreen* m_pInstance;
};
#endif // PROTOSCREEN_H
