#ifdef _DEBUG
#define LEPUS_ALLOW_STDOUT
#endif

#include "DemoApp.h"

using namespace lepus;

#ifndef _DEBUG
#define NDEBUG
#endif


int main()
{
	DemoApp app;

	return app.Run();
}