#ifndef LSYSTEM_IO_FILESYSTEM
#define LSYSTEM_IO_FILESYSTEM

#include <fstream>
#include <cstring>

namespace lepus
{
    namespace system
    {
	/**
	 * Helper class for working with files on the file system.
	 */
	class FileSystem
	{
	    public:
	    /**
	     * Reads all text from the specified file.
	     * @param path Path string to the file to read.
	     * @return A std::string that contains all text from the file.
	     */
	    static std::string Read(const char* path);

	    /**
	     * Reads file contents as binary.
	     * @tparam TData Type of each element in the array. By default, this is a std::byte, resulting in a byte array.
	     * @param path The path of the file to read
	     * @param size Total size read (in bytes)
	     * @return
	     */
	    template <typename TData = std::byte>
	    static TData* ReadBinary(const char* path, size_t& size)
	    {
		auto reader = std::basic_ifstream<std::byte>(path, std::ifstream::in | std::ifstream::binary);

		constexpr size_t bufferSz = 1024;
		std::byte buffer[bufferSz] = {};
		memset(buffer, 0, bufferSz);

		auto* bytes = new std::byte[bufferSz];

		size = 0;
		while (reader.good())
		{
		    reader.read(buffer, bufferSz);
		    auto szRead = static_cast<size_t>(reader.gcount());

		    auto* temp = new std::byte[size + szRead];
		    memmove(temp, bytes, size);
		    delete[] bytes;
		    memmove(temp + size, buffer, szRead);
		    bytes = temp;

		    size += szRead;
		}

		reader.close();

		return reinterpret_cast<TData*>(bytes);
	    }
	};
    } // namespace system
} // namespace lepus

#endif