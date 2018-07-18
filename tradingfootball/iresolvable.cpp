#include "iresolvable.h"
#include "core.h"
IResolvable::IResolvable()
{}

IResolvable::~IResolvable()
{
    Core::unregisterInstance(this);
}

