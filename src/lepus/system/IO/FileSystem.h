#ifndef LSYSTEM_IO_FILESYSTEM
#define LSYSTEM_IO_FILESYSTEM

#include <fstream>

namespace lepus
{
	namespace system
	{
		class FileSystem
		{
			public:
			static std::string Read(const char* path);
		};
	}
}

#endif