#ifndef LSYSTEM_WINDOWING
#define LSYSTEM_WINDOWING

#include <memory>

namespace lepus
{
	namespace system
	{
		class Windowing
		{
			protected:
			/// @brief Window dimensions (in pixels).
			struct
			{
				unsigned short width = 0;
				unsigned short height = 0;
			} m_Dimensions;
			public:
			unsigned short GetWindowWidth() { return m_Dimensions.width; }
			unsigned short GetWindowHeight() { return m_Dimensions.height; }

			virtual void SetWindowDimensions(unsigned short width, unsigned short height) = 0;

			/// @brief Obtains a pointer to the window handle itself.
			/// This will be API/OS-specific, so the caller needs to be aware of the expected window handle type.
			/// 
			/// @return A raw pointer to the window object/handle, cast to a void pointer to make it platform/API-agnostic.
			virtual void* GetWindowPtr() = 0;

			/// @brief Performs back/front buffer swap. This usually needs to be called after rendering a frame.
			virtual void SwapBuffers() = 0;
		};
	}
}

#endif