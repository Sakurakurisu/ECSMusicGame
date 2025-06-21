#pragma once

#include <mutex>
#include <memory>
#include <iostream>
#include <type_traits>

namespace GameFramework
{
	// 辅助类，用于检查构造函数是否为受保护或私有
	template <typename T>
	class CheckProtectedConstructor {
	private:
		// 尝试访问构造函数（如果构造函数是私有的或受保护的，这里会失败）
		template <typename U>
		static auto test(int) -> decltype(U(), std::false_type{});

		// Fallback，如果构造函数是受保护的或私有的，则这里会匹配
		template <typename>
		static std::true_type test(...) { };

	public:
		// 如果 `test<int>(0)` 返回 `true_type`，则类具有私有或受保护的构造函数
		static constexpr bool value = decltype(test<T>(0))::value;
	};

	// BaseManager 单例模式基类
	template<typename T>
	class BaseManager {
	private:
		static T* instance;
		static std::mutex mutex;

	protected:
		BaseManager()
		{
			// 在构造函数中使用 static_assert 检查
			static_assert(CheckProtectedConstructor<T>::value, "T must have a protected or private constructor");
		}
		virtual ~BaseManager() = default;

	public:
		// 禁用拷贝构造函数和赋值操作符
		BaseManager(const BaseManager&) = delete;
		BaseManager& operator=(const BaseManager&) = delete;

		static T& Instance() {
			if (instance == nullptr)
			{
				std::lock_guard<std::mutex> lock(mutex);
				if (instance == nullptr)
					instance = new T();
			}
			return *instance;
		}
	};

	// 初始化静态成员
	template<typename T>
	T* BaseManager<T>::instance = nullptr;

	template<typename T>
	std::mutex BaseManager<T>::mutex;
}
