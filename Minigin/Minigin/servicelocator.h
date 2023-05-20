#pragma once
namespace dae
{
	template <typename T>
	class servicelocator final
	{
	public:
		static T& get_serviceLocator()
		{
			return *instance_;
		}

		static void register_service_locator(T* ss)
		{
			instance_ = ss;
		}

	private:
		static T* instance_;
	};

	template <typename T>
	T* servicelocator<T>::instance_ = nullptr;
}

