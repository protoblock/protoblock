#include "colors.h"

Colors::Colors(QObject *parent):
    QObject(parent),
    m_theme(Material)
{
    connect(this,SIGNAL(themeChanged()),this,SLOT(resetTheme()));
    init();

    m_primaryColor = blue ();
    m_accentColor = amber ();
    m_tabHighlightColor = white ();
}

Colors::~Colors()
{
}

Colors::Theme Colors::theme() const
{
    return m_theme;
}

void Colors::setTheme(const Colors::Theme &theme)
{
    if ( m_theme == theme )
        return;
    m_theme = theme ;
    themeChanged();
}

QStringList Colors::themeNames() const
{
    return m_themeNames;
}

void Colors::setThemeNames()
{
    m_themeNames.clear();
    QList<QString> li;
    li << "Ubuntu"  << "Material" <<  "Flat" <<  "Metro" <<  "Mappel";
    m_themeNames = li;
}

QString Colors::red() const
{
    return m_red;
}

QString Colors::pink() const
{
    return m_pink;
}

QString Colors::purple() const
{
    return m_purple;
}

QString Colors::deepPurple() const
{
    return m_deepPurple;
}

QString Colors::indigo() const
{
    return m_indigo;
}

QString Colors::blue() const
{
    return m_blue;
}

QString Colors::lightBlue() const
{
    return m_lightBlue;
}

QString Colors::cyan() const
{
    return m_cyan;
}

QString Colors::teal() const
{
    return m_teal;
}

QString Colors::green() const
{
    return m_green;
}

QString Colors::lightGreen() const
{
    return m_lightGreen;
}

QString Colors::lime() const
{
    return m_lime;
}

QString Colors::yellow() const
{
    return m_yellow;
}

QString Colors::amber() const
{
    return m_amber;
}

QString Colors::orange() const
{
    return m_orange;
}

QString Colors::deepOrange() const
{
    return m_deepOrange ;
}

QString Colors::brown() const
{
    return m_brown;
}

QString Colors::grey() const
{
    return m_grey;
}

QString Colors::blueGrey() const
{
    return m_blueGrey;
}

QString Colors::white() const
{
    return m_white;
}

QString Colors::black() const
{
    return m_black;
}

void Colors::setPink()
{
    switch (m_theme) {
    case Material:
        m_pink = "#E91E63";
        break;
    case Ubuntu:
        m_pink = "#E91E63";
        break;
    case Flat:
        m_pink = "E08283";
        break;
    case Metro:
        m_pink = "#ff0097";
        break;
    case Mappel:
        m_pink = "#D9277A";
        break;
    }
}

void Colors::setPurple()
{
    switch (m_theme) {
    case Material:
        m_purple = "#9C27B0";
        break;
    case Ubuntu:
        m_purple = "#762572";
        break;
    case Flat:
        m_purple = "#9A12B3";
        break;
    case Metro:
        m_purple = "#7e3878";
        break;
    case Mappel:
        m_purple = "#7e3878";
        break;
    }
}

void Colors::setDeepPurple()
{
    switch (m_theme) {
    case Material:
        m_deepPurple = "#673AB7";
        break;
    case Ubuntu:
        m_deepPurple = "#2C001E";
        break;
    case Flat:
        m_deepPurple = "#663399";
        break;
    case Metro:
        m_deepPurple = "#603cba";
        break;
    case Mappel:
        m_deepPurple = "#603cba";
        break;
    }
}

void Colors::setIndigo()
{
    switch (m_theme) {
    case Material:
        m_indigo = "#3F51B5";
        break;
    case Ubuntu:
        m_indigo = "#77216F";
        break;
    case Flat:
        m_indigo = "#4E2A7C";
        break;
    case Metro:
        m_indigo = "#6A00FF";
        break;
    case Mappel:
        m_indigo = "#4E2A7C";
        break;
    }
}

void Colors::setBlue()
{
    switch (m_theme) {
    case Material:
        m_blue = "#2196F3";
        break;
    case Ubuntu:
        m_blue = "#12a3d8";
        break;
    case Flat:
        m_blue = "#446CB3";
        break;
    case Metro:
        m_blue = "#00AFF0";
        break;
    case Mappel:
        m_blue = "#197CCC";
        break;
    }
}

void Colors::setLightBlue()
{
    switch (m_theme) {
    case Material:
        m_lightBlue = "#03A9F4";
        break;
    case Ubuntu:
        m_lightBlue = "#2D89EF";
        break;
    case Flat:
        m_lightBlue = "#22A7F0";
        break;
    case Metro:
        m_lightBlue = "#4390DF";
        break;
    case Mappel:
        m_lightBlue = "#5AC8FA";
        break;
    }
}

void Colors::setCyan()
{
    switch (m_theme) {
    case Material:
        m_cyan = "#00BCD4";
        break;
    case Ubuntu:
        m_cyan = "#00BCD4";
        break;
    case Flat:
        m_cyan = "#1BA1E2";
        break;
    case Metro:
        m_cyan = "#1BA1E2";
        break;
    case Mappel:
        m_cyan = "#5AC8FA";
        break;
    }
}

void Colors::setTeal()
{
    switch (m_theme) {
    case Material:
        m_teal = "#009688";
        break;
    case Ubuntu:
        m_teal = "#009688";
        break;
    case Flat:
        m_teal = "#00ABA9";
        break;
    case Metro:
        m_teal = "#00ABA9";
        break;
    case Mappel:
        m_teal = "#00ABA9";
        break;
    }
}

void Colors::setGreen()
{
    switch (m_theme) {
    case Material:
        m_green = "#4CAF50";
        break;
    case Ubuntu:
        m_green = "#00a132";
        break;
    case Flat:
        m_green = "#00B16A";
        break;
    case Metro:
        m_green = "#60A917";
        break;
    case Mappel:
        m_green = "#4CD964";
        break;
    }
}

void Colors::setLightGreen()
{
    switch (m_theme) {
    case Material:
        m_lightGreen = "#8BC34A";
        break;
    case Ubuntu:
        m_lightGreen = "#4ad35c";
        break;
    case Flat:
        m_lightGreen = "#7AD61D";
        break;
    case Metro:
        m_lightGreen = "#7AD61D";
        break;
    case Mappel:
        m_lightGreen = "#7AD61D";
        break;
    }
}

void Colors::setLime()
{
    switch (m_theme) {
    case Material:
        m_lime = "#CDDC39";
        break;
    case Ubuntu:
        m_lime = "#CDDC39";
        break;
    case Flat:
        m_lime = "#CDDC39";
        break;
    case Metro:
        m_lime = "#CDDC39";
        break;
    case Mappel:
        m_lime = "#CDDC39";
        break;
    }
}

void Colors::setYellow()
{
    switch (m_theme) {
    case Material:
        m_yellow = "#FFEB3B";
        break;
    case Ubuntu:
        m_yellow = "#f1c40f";
        break;
    case Flat:
        m_yellow = "#f1c40f";
        break;
    case Metro:
        m_yellow = "#E3C800";
        break;
    case Mappel:
        m_yellow = "#FFCC00";
        break;
    }
}

void Colors::setAmber()
{
    switch (m_theme) {
    case Material:
        m_amber = "#FFC107";
        break;
    case Ubuntu:
        m_amber = "#e67e22";
        break;
    case Flat:
        m_amber = "#e67e22";
        break;
    case Metro:
        m_amber = "#F0A30A";
        break;
    case Mappel:
        m_amber = "#e67e22";
        break;
    }
}

void Colors::setOrange()
{
    switch (m_theme) {
    case Material:
        m_orange = "#FF9800";
        break;
    case Ubuntu:
        m_orange = "#DD4814";
        break;
    case Flat:
        m_orange = "#f39c12";
        break;
    case Metro:
        m_orange = "#FA6800";
        break;
    case Mappel:
        m_orange = "#FF9500";
        break;
    }
}

void Colors::setDeepOrange()
{
    switch (m_theme) {
    case Material:
        m_deepOrange = "#FF5722";
        break;
    case Ubuntu:
        m_deepOrange = "#D35400";
        break;
    case Flat:
        m_deepOrange = "#D35400";
        break;
    case Metro:
        m_deepOrange = "#BF5A15";
        break;
    case Mappel:
        m_deepOrange = "#d67c00";
        break;
    }
}

void Colors::setBrown()
{
    switch (m_theme) {
    case Material:
        m_brown = "#795548";
        break;
    case Ubuntu:
        m_brown = "#825A2C";
        break;
    case Flat:
        m_brown = "#825A2C";
        break;
    case Metro:
        m_brown = "#825A2C";
        break;
    case Mappel:
        m_brown = "#825A2C";
        break;
    }
}

void Colors::setGrey()
{
    switch (m_theme) {
    case Material:
        m_grey = "#9E9E9E";
        break;
    case Ubuntu:
        m_grey = "#888888";
        break;
    case Flat:
        m_grey = "#95a5a6";
        break;
    case Metro:
        m_grey = "#555555";
        break;
    case Mappel:
        m_grey = "#555555";
        break;
    }
}

void Colors::setBlueGrey()
{
    switch (m_theme) {
    case Material:
        m_blueGrey = "#607D8B";
        break;
    case Ubuntu:
        m_blueGrey = "#607D8B";
        break;
    case Flat:
        m_blueGrey = "#718e9b";
        break;
    case Metro:
        m_blueGrey = "#718e9b";
        break;
    case Mappel:
        m_blueGrey = "#718e9b";
        break;
    }
}

void Colors::setWhite()
{
    m_white = "#FFFFFF";
}

void Colors::setBlack()
{
    m_black = "#000000";
}

void Colors::setRed()
{
    switch (m_theme) {
    case Material:
        m_red = "#F44336";
        break;
    case Ubuntu:
        m_red = "#f32c36";
        break;
    case Flat:
        m_red = "#c0392b";
        break;
    case Metro:
        m_red = " #ee1111";
        break;
    case Mappel:
        m_red = "#FF3B30";
        break;
    }
}

void Colors::resetTheme()
{
    init();
}

void Colors::init()
{
    setRed();
    setPink();
    setPurple();
    setDeepPurple();
    setIndigo();
   setBlue();
    setLightBlue();
    setCyan();
    setTeal();
    setGreen();
    setLightGreen();
    setLime();
    setYellow();
    setAmber();
    setOrange();
    setDeepOrange();
    setBrown();
    setGrey();
    setBlueGrey();
    setWhite();
    setBlack();




}
