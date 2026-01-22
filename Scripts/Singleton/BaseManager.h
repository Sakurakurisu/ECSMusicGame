#pragma once

#include <mutex>
#include <memory>
#include <iostream>
#include <type_traits>

namespace GameFramework
{
	// コンストラクタがprotected/privateかを検証するヘルパークラス（SFINAE利用）
	template <typename T>
	class CheckProtectedConstructor {
	private:
		// コンストラクタがpublicならマッチ → false_type
		template <typename U>
		static auto test(int) -> decltype(U(), std::false_type{});

		// コンストラクタがprotected/privateならこちらにフォールバック → true_type
		template <typename>
		static std::true_type test(...) { };

	public:
		static constexpr bool value = decltype(test<T>(0))::value;
	};

	// シングルトンパターン基底クラス（CRTPで派生クラスTを受け取る）
	template<typename T>
	class BaseManager {
	private:
		static T* instance;
		static std::mutex mutex;

	protected:
		BaseManager()
		{
			// 派生クラスのコンストラクタがprotected/privateであることを保証
			static_assert(CheckProtectedConstructor<T>::value, "T must have a protected or private constructor");
		}
		virtual ~BaseManager() = default;

	public:
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

	template<typename T>
	T* BaseManager<T>::instance = nullptr;

	template<typename T>
	std::mutex BaseManager<T>::mutex;
}
