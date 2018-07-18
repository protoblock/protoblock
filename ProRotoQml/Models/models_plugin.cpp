#include "models_plugin.h"
#include <qqml.h>
#include "sortfilterproxymodel.h"

void ProRotoQmlModelsPlugin::registerTypes(const char *uri)
{
    // @uri ProRotoQml.Models
    qmlRegisterType<SortFilterProxyModel>(uri,1,0,"SortModel");
}

