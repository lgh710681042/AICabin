#pragma once

#include <memory>
#include <mutex>

using namespace std;

namespace Util
{
	template <typename T>
	class Singleton
	{
	public:
		static shared_ptr<T> GetInstance();

		Singleton(const Singleton& p) = delete;
		Singleton& operator=(const Singleton& p) = delete;

		Singleton() {}
		~Singleton() {}

	private:
		template<class K>
		class singleton_wrapper : public K
		{
		public:
			~singleton_wrapper()
			{

			}
		};
		static std::mutex s_mutex;
		static shared_ptr<T> s_instance;
	};

	template <typename T>
	std::mutex Singleton<T>::s_mutex;

	template <typename T>
	shared_ptr<T> Singleton<T>::s_instance;

	template <typename T>
	shared_ptr<T> Singleton<T>::GetInstance()
	{
		if (!s_instance)
		{
			std::lock_guard<std::mutex> guard(s_mutex);
			if (!s_instance)
			{
				s_instance = make_shared<singleton_wrapper<T>>();
			}
		}

		return s_instance;
	}
}