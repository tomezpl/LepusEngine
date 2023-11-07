#include "../FileSystem.h"
#include <cstring>

using namespace LepusEngine::LepusSystem;

std::string FileSystem::Read(const char* path)
{
	std::string contents = "";

	std::ifstream reader = std::ifstream(path, std::ifstream::in);

	const size_t bufSz = 1024;
	char buffer[bufSz + 1];
	memset(buffer, 0, bufSz + 1);

	while (reader.good())
	{
		reader.read(buffer, bufSz);

		if (reader.gcount() <= 0)
		{
			break;
		}
		else
		{
			contents.append(buffer);
		}

		memset(buffer, 0, bufSz + 1);
	}

	reader.close();

	return contents;
}
