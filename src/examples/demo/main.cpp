#define LEPUS_FORCE_API_OPENGL 1

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