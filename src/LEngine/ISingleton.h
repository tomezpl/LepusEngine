#ifndef LENGINE_SINGLETON
#define LENGINE_SINGLETON

namespace LepusEngine
{
	template <typename TObject>
	class ISingleton
	{
	public:
		static virtual const TObject& Singleton() = 0;
	};
}

#endif