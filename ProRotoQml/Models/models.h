#ifndef MODELS_H
#define MODELS_H

#include <QQuickItem>

class Models : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Models)

public:
    Models(QQuickItem *parent = 0);
    ~Models();
};

#endif // MODELS_H
