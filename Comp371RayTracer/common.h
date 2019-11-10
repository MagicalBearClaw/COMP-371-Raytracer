#pragma once

// macros to safely delete memory.

#define SAFE_DELETE(x)  \
if (x != nullptr)       \
{                       \
	delete x;           \
}                       \

#define SAFE_DELETE_ARRAY(x)  \
if (x != nullptr)             \
{                             \
	delete[] x;               \
}                             \


