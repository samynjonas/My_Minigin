#pragma once
#include <memory>

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

		static void register_service_locator(std::unique_ptr<T> ss)
		{
			instance_ = std::move(ss);
		}

	private:
		static std::unique_ptr<T> instance_;
	};

	template <typename T>
	std::unique_ptr<T> servicelocator<T>::instance_ = nullptr;
}

