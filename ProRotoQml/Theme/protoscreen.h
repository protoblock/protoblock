#ifndef PROTOSCREEN_H
#define PROTOSCREEN_H

#include <QObject>
#include <QtQml>
#include <QObject>
#include <QtQml>
#include <QScreen>
#include <QSysInfo>

#include "QQmlConstRefPropertyHelpers.h"


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
    Q_PROPERTY (QString formFactor READ formFactor NOTIFY formFactorChanged)
    Q_ENUMS( ProtoFont )



    QML_READONLY_CSTREF_PROPERTY ( int, availableHeight)
    QML_READONLY_CSTREF_PROPERTY ( int, availableWidth)
    QML_READONLY_CSTREF_PROPERTY ( qreal, pixelRatio );


public:

    explicit ProtoScreen(QObject *parent = 0);
    QString formFactor()const
    {
        return m_formFactor;
    }
    enum ProtoFont {
        NOTSET,
        XXLARGE ,
        XLARGE ,
        LARGE ,
        MEDIUM ,
        NORMAL ,
        SMALL ,
        TINY
    };



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

    bool isInitialized() { return m_bInitialized; }

    void setGridUnit(const double &unit);
    double gridUnit() { return m_gridUnit; }

    int desktopWidth() { return m_desktopGeometry.width(); }
    int desktopHeight() { return m_desktopGeometry.height(); }

    int designWidth() { return m_designResolution.width(); }
    int designHeight() { return m_designResolution.height(); }
    double displaySize() { return m_displayDiagonalSize; }
    double devicePixelRatio() { return m_devicePixelRatio; }

    void finalFormFactor(const QString &systemType, const double &versionORscaleSize , const double diagonal);
    double checkIphoneScaleSize(const int &width, const int &height ,const double &iPhoneVersion);
    void updateFormFactor();

    double scaleSize()const;
    void setScaleSize(const double &size);
    void updateFonts();
    Q_INVOKABLE double guToPx(double units);
    Q_INVOKABLE double pxToGu(double px);
//    Q_INVOKABLE void setDesignResolution(int width, int height);
    Q_INVOKABLE double font(ProtoFont fontSize) {
        return m_fonts[fontSize];
    }
    void initialize();





signals:
    void gridUnitChanged();
    void scaleSizeChanged();
    void formFactorChanged();
private:
    //            double desktopDistanceToDisplay = 0.5;
    //            int desktopPixelDensity = 112;
    //            double tvDistanceToDisplay = 2.0;
    //            int tvPixelDensity = 52; // 1080p on a 42" screen
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
    QString m_androidDpi;
    double m_windowsDesktopScale;
    double m_androidScale;
    double m_tempMacVersion;
};
#endif // PROTOSCREEN_H
