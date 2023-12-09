#ifndef LUTILITY_TYPES_VIEWPORT
#define LUTILITY_TYPES_VIEWPORT

namespace lepus
{
	namespace types
	{
		struct Viewport
		{
			int x = 0, y = 0, width = 0, height = 0;
		};

		struct ViewportF
		{
			float x = 0, y = 0, width = 0, height = 0;
		};
	}
}

#endif