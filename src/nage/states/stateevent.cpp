// Copyright (C) 2014-2015 Eric Hebert (ayebear)
// This code is licensed under LGPLv3, see LICENSE.txt for details.

#include "nage/states/stateevent.h"

namespace ng
{

void StateEvent::reset()
{
    command = Continue;
    name.clear();
}

}