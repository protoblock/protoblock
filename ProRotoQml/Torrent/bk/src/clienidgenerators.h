#ifndef CLIENIDGENERATORS_H
#define CLIENIDGENERATORS_H

#include <QByteArray>
#include <QHash>
#include <QSharedPointer>

namespace IdStyles {
enum Styles{
    Unknown,
    Shadow,
    Azareus,
    Custom,
};
}

struct IClientIdGenerator{
    virtual QByteArray generate() = 0;
    virtual ~IClientIdGenerator(){}
};

typedef QSharedPointer< IClientIdGenerator > ClientIdGeneratorPtr;

class ClientIdGeneratorsFactory
{
public:
    static ClientIdGeneratorsFactory *instance();

    ClientIdGeneratorPtr getGenerator( IdStyles::Styles style );

private:
    QHash< int, ClientIdGeneratorPtr > m_Cache;
};


class ClientIdGeneratorShadow : public IClientIdGenerator
{
public:
    virtual QByteArray generate() override;
};

class ClientIdGeneratorBram : public IClientIdGenerator
{
public:
    virtual QByteArray generate() override;
};

class ClientIdGeneratorCustom : public IClientIdGenerator
{
public:
    virtual QByteArray generate() override;
};

#endif // CLIENIDGENERATORS_H
