#ifndef LSYSTEM_IO_FILESYSTEM
#define LSYSTEM_IO_FILESYSTEM

#include <fstream>
#include <cstring>

namespace lepus
{
    namespace system
    {
	class FileSystem
	{
	    public:
	    static std::string Read(const char* path);
	    template <typename TData = char>
	    static TData* ReadBinary(const char* path, size_t& size)
	    {
		std::ifstream reader = std::ifstream(path, std::ifstream::in | std::ifstream::binary);

		const size_t bufferSz = 1024;
		char buffer[bufferSz] = {};
		memset(buffer, 0, bufferSz);

		char* bytes = new char[bufferSz];

		size = 0;
		while (reader.good())
		{
		    reader.read(buffer, bufferSz);
		    size_t szRead = static_cast<size_t>(reader.gcount());

		    char* temp = new char[size + szRead];
		    memmove((void*)temp, (void*)bytes, size);
		    delete[] bytes;
		    memmove((void*)(temp + size), (void*)buffer, szRead);
		    bytes = temp;

		    size += szRead;
		}

		reader.close();

		return (TData*)bytes;
	    }
	};
    } // namespace system
} // namespace lepus

#endif