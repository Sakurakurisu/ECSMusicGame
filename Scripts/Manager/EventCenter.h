#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include "BaseManager.h"

namespace GameFramework
{
	// イベントタイプの列挙体
	enum class E_EventType
	{
		EVENT_TYPE_1,
		EVENT_TYPE_2,
		// より多くのイベントタイプを追加
	};

	// 基本のイベント情報クラス、すべてのイベント情報クラスの基底クラス
	class EventInfoBase
	{
	public:
		virtual ~EventInfoBase() = default;
	};

	// 引数ありイベント情報クラス
	template<typename T>
	class EventInfo : public EventInfoBase
	{
	public:
		std::vector<std::function<void(T)>> actions;

		void AddAction(const std::function<void(T)>& action);
		void Invoke(T info);
	};

	// 引数なしイベント情報クラス
	template<>
	class EventInfo<void> : public EventInfoBase
	{
	public:
		std::vector<std::function<void()>> actions;

		void AddAction(const std::function<void()>& action);
		void Invoke();
	};

	// イベントセンタークラス
	class EventCenter : public BaseManager<EventCenter>
	{
		friend class BaseManager<EventCenter>; // BaseManager にプライベートコンストラクタへのアクセスを許可

	public:
		// 引数ありイベントを発火
		template<typename T>
		void EventTrigger(E_EventType eventName, T info);

		// 引数なしイベントを発火
		void EventTrigger(E_EventType eventName);

		// 引数ありのイベントリスナーを追加
		template<typename T>
		void AddEventListener(E_EventType eventName, const std::function<void(T)>& func);

		// 引数なしのイベントリスナーを追加
		void AddEventListener(E_EventType eventName, const std::function<void()>& func);

		// 引数ありのイベントリスナーを削除
		template<typename T>
		void RemoveEventListener(E_EventType eventName, const std::function<void(T)>& func);

		// 引数なしのイベントリスナーを削除
		void RemoveEventListener(E_EventType eventName, const std::function<void()>& func);

		// すべてのイベントリスナーをクリア
		void ClearAll();

		// 指定したイベントのすべてのリスナーをクリア
		void Clear(E_EventType eventName);

	private:
		EventCenter() = default; // 保護されたコンストラクタ
		~EventCenter() = default;
		std::unordered_map<E_EventType, std::shared_ptr<EventInfoBase>> eventDic;
	};

	// テンプレート関数の定義（テンプレートはヘッダファイル内で定義が必要）
	template<typename T>
	void EventInfo<T>::AddAction(const std::function<void(T)>& action)
	{
		actions.push_back(action);
	}

	template<typename T>
	void EventInfo<T>::Invoke(T info)
	{
		for (auto& action : actions)
		{
			action(info);
		}
	}

	template<typename T>
	void EventCenter::EventTrigger(E_EventType eventName, T info)
	{
		if (eventDic.count(eventName))
		{
			auto eventInfo = std::dynamic_pointer_cast<EventInfo<T>>(eventDic[eventName]);
			if (eventInfo)
			{
				eventInfo->Invoke(info);
			}
		}
	}

	template<typename T>
	void EventCenter::AddEventListener(E_EventType eventName, const std::function<void(T)>& func)
	{
		if (eventDic.count(eventName))
		{
			auto eventInfo = std::dynamic_pointer_cast<EventInfo<T>>(eventDic[eventName]);
			if (eventInfo)
			{
				eventInfo->AddAction(func);
			}
		}
		else
		{
			auto eventInfo = std::make_shared<EventInfo<T>>();
			eventInfo->AddAction(func);
			eventDic[eventName] = eventInfo;
		}
	}

	template<typename T>
	void EventCenter::RemoveEventListener(E_EventType eventName, const std::function<void(T)>& func)
	{
		if (eventDic.count(eventName))
		{
			auto eventInfo = std::dynamic_pointer_cast<EventInfo<T>>(eventDic[eventName]);
			if (eventInfo)
			{
				auto it = std::find_if(eventInfo->actions.begin(), eventInfo->actions.end(),
									   [&func](const std::function<void(T)>& f)
									   {
										   return f.target_type() == func.target_type() && *f.target<void(*)(T)>() == *func.target<void(*)(T)>();
									   });
				if (it != eventInfo->actions.end())
				{
					eventInfo->actions.erase(it);
				}
			}
		}
	}
}
