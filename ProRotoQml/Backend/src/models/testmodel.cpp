#include "testmodel.h"
#include "../QmlModels/QQmlObjectListModel.h"

TestModelItem::TestModelItem(const QDateTime &when, QObject *parent)
    : QObject    (parent)
    , m_day      (when.date ().day ())
    , m_month    (when.toString ("MMMM"))
    , m_year     (when.date ().year ())
    , m_selected (false)
    , m_str      ("Hello World !")
{
}

void TestModelItem::setModelUp(){
    QQmlObjectListModel<TestModelItem> model;
     for (int offset = 0; offset < 100; offset++) {
         model.append (new TestModelItem (QDateTime::currentDateTimeUtc ().addDays (offset)));
     }
}
