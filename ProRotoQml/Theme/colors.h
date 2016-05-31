#ifndef YACM_H
#define YACM_H

#include <QObject>
#include <QString>

class Colors : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString red READ red NOTIFY redChanged())
    Q_PROPERTY(QString pink READ pink  NOTIFY pinkChanged)
    Q_PROPERTY(QString purple READ purple  NOTIFY purpleChanged)
    Q_PROPERTY(QString deepPurple READ deepPurple  NOTIFY deepPurpleChanged)
    Q_PROPERTY(QString indigo READ indigo  NOTIFY indigoChanged)
    Q_PROPERTY(QString blue READ blue  NOTIFY blueChanged)
    Q_PROPERTY(QString lightBlue READ lightBlue NOTIFY lightBlueChanged)
    Q_PROPERTY(QString cyan READ cyan  NOTIFY cyanChanged)
    Q_PROPERTY(QString teal READ teal  NOTIFY tealChanged)
    Q_PROPERTY(QString green READ green  NOTIFY greenChanged)
    Q_PROPERTY(QString lightGreen READ lightGreen  NOTIFY lightGreenChanged)
    Q_PROPERTY(QString lime READ lime  NOTIFY limeChanged)
    Q_PROPERTY(QString yellow READ yellow NOTIFY yellowChanged)
    Q_PROPERTY(QString amber READ amber  NOTIFY amberChanged)
    Q_PROPERTY(QString orange READ orange NOTIFY orangeChanged)
    Q_PROPERTY(QString deepOrange READ deepOrange  NOTIFY deepOrangeChanged)
    Q_PROPERTY(QString brown READ brown  NOTIFY brownChanged)
    Q_PROPERTY(QString grey READ grey NOTIFY greyChanged)
    Q_PROPERTY(QString  blueGrey READ blueGrey NOTIFY blueGreyChanged)
    Q_PROPERTY(QString white READ white NOTIFY whiteChanged)
    Q_PROPERTY(QString black READ black NOTIFY blackChanged)

    Q_PROPERTY(QStringList themeNames READ themeNames  NOTIFY themeNamesChanged)


    Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_ENUMS(Theme)



public:
    Colors(QObject *parent = 0);
    ~Colors();

    enum   Theme{
        Ubuntu,
        Material,
        Flat,
        Metro,
        Mappel
    };
    Theme theme()const;
    void setTheme(const Theme &theme);
    QStringList themeNames()const;
    void setThemeNames();


    QString red()const;
    QString pink()const;
    QString purple()const;
    QString deepPurple()const;
    QString indigo()const;
    QString blue()const;
    QString lightBlue()const;
    QString cyan()const;
    QString teal()const;
    QString green()const;
    QString lightGreen()const;
    QString lime()const;
    QString yellow()const;
    QString amber()const;
    QString orange()const;
    QString deepOrange()const;
    QString brown()const;
    QString grey()const;
    QString blueGrey()const;
    QString white()const;
    QString black()const;

    // setters
    void setRed();
    void setPink();
    void setPurple();
    void setDeepPurple();
    void setIndigo();
    void setBlue();
    void setLightBlue();
    void setCyan();
    void setTeal();
    void setGreen();
    void setLightGreen();
    void setLime();
    void setYellow();
    void setAmber();
    void setOrange();
    void setDeepOrange();
    void setBrown();
    void setGrey();
    void setBlueGrey();
    void setWhite();
    void setBlack();


    void init();

signals:
    void redChanged();
    void pinkChanged();
    void purpleChanged();
    void deepPurpleChanged();
    void indigoChanged();
    void blueChanged();
    void  lightBlueChanged();
    void cyanChanged();
    void tealChanged();
    void greenChanged();
    void lightGreenChanged();
    void limeChanged();
    void yellowChanged();
    void amberChanged();
    void orangeChanged();
    void deepOrangeChanged();
    void brownChanged();
    void greyChanged();
    void blueGreyChanged();
    void whiteChanged();
    void blackChanged();

    void themeNamesChanged();
    void themeChanged();

protected slots:
    void resetTheme();

private:
    Theme m_theme;
    QStringList m_themeNames;

    QString m_red;
    QString m_pink;
    QString m_purple;
    QString m_deepPurple;
    QString m_indigo;
    QString m_blue;
    QString m_lightBlue;
    QString m_cyan;
    QString m_teal;
    QString m_green;
    QString m_lightGreen;
    QString m_lime;
    QString m_yellow;
    QString m_amber;
    QString m_orange;
    QString m_deepOrange;
    QString m_brown;
    QString m_grey;
    QString m_blueGrey;
    QString m_white;
    QString m_black;

};

#endif // COLORS_H
