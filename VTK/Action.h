#ifndef __ACTION_H
#define __ACTION_H

#include "window.h"

class Action
{
public:
	virtual void Execute(Message& message) = 0;
};

#endif