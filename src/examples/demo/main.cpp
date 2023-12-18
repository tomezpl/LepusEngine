#ifdef _DEBUG
#define LEPUS_ALLOW_STDOUT
#endif

#include "DemoApp.h"

using namespace lepus;

int main()
{
	DemoApp app;

	return app.Run();
}