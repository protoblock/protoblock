#ifndef PROTOSCREEN_H
#define PROTOSCREEN_H

#include <QObject>
#include <QtQml>
#include <QObject>
#include <QtQml>
#include <QScreen>
#include <QSysInfo>


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
    Q_PROPERTY(double scaleSize READ scaleSize NOTIFY scaleSizeChanged )
    Q_ENUMS( ProtoFont )

public:

    explicit ProtoScreen(QObject *parent = 0);

    enum ProtoFont {
        FONT_NOTSET = 0,
        FONT_XXLARGE =1 ,
        FONT_XLARGE = 2,
        FONT_LARGE = 3,
        FONT_MEDIUM = 4,
        FONT_NORMAL = 5,
        FONT_SMALL = 6,
        FONT_TINY = 7
    };

    static QObject *singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static ProtoScreen* instance(QQmlEngine *engine);


    //            double desktopDistanceToDisplay = 0.5;
    //            int desktopPixelDensity = 112;


    //            double tvDistanceToDisplay = 2.0;
    //            int tvPixelDensity = 52; // 1080p on a 42" screen

    //            /* distanceToDisplay and pixelDensity cannot be global javascript variables
    //                   because they would not be able to access m_formFactor
    //                */
    //            int dtPx(const int &desktopPixels) {
    //                auto  distanceToDisplay = m_formFactor === "desktop" ? desktopDistanceToDisplay : tvDistanceToDisplay;
    //                auto pixelDensity = m_formFactor === "desktop" ? desktopPixelDensity : tvPixelDensity;
    //                auto  factorFromDesktop = pixelDensity / desktopPixelDensity * distanceToDisplay / desktopDistanceToDisplay;
    //                return (desktopPixels * factorFromDesktop)
    //            }
    //            int tvPx(const int &tvPixels)
    //            {
    //                auto distanceToDisplay = m_formFactor === "desktop" ? desktopDistanceToDisplay : tvDistanceToDisplay;'
    //                auto  pixelDensity = m_formFactor === "desktop" ? desktopPixelDensity : tvPixelDensit;
    //                auto factorFromTv = pixelDensity / tvPixelDensity * distanceToDisplay / tvDistanceToDispla;
    //                return (tvPixels * factorFromTv);
    //            }

    //            int phonePx(const int &phonePixels){

    //            }


    void finalFormFactor(const QString &systemType, const double &versionORscaleSize , const double diagonal);
    double checkIphoneScaleSize(const int &width, const int &height ,const double &iPhoneVersion);
    void updateFormFactor();

    double scaleSize()const;
    void setScaleSize(const double &size);

    Q_INVOKABLE double guToPx(double units);
    Q_INVOKABLE double pxToGu(double px);
//    Q_INVOKABLE void setDesignResolution(int width, int height);
    Q_INVOKABLE double font(ProtoFont fontSize) { return m_fonts[fontSize]; }

signals:
    void gridUnitChanged();
    void scaleSizeChanged();

protected slots:
    void updateFonts();
//    void updateGridUnit();

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


    QMap<ProtoFont, double> m_fonts;

    double m_scaleSize;
    QString m_formFactor;

    static ProtoScreen* m_pInstance;
};
#endif // PROTOSCREEN_H
