#include "EventCenter.h"
#include <algorithm>

namespace GameFramework
{
	void EventInfo<void>::AddAction(const std::function<void()>& action)
	{
		actions.push_back(action);
	}

	void EventInfo<void>::Invoke()
	{
		for (auto& action : actions)
		{
			action();
		}
	}

	void EventCenter::EventTrigger(E_EventType eventName)
	{
		if (eventDic.count(eventName))
		{
			auto eventInfo = std::dynamic_pointer_cast<EventInfo<void>>(eventDic[eventName]);
			if (eventInfo) {
				eventInfo->Invoke();
			}
		}
	}

	void EventCenter::AddEventListener(E_EventType eventName, const std::function<void()>& func)
	{
		if (eventDic.count(eventName))
		{
			auto eventInfo = std::dynamic_pointer_cast<EventInfo<void>>(eventDic[eventName]);
			if (eventInfo)
				eventInfo->AddAction(func);
		}
		else
		{
			auto eventInfo = std::make_shared<EventInfo<void>>();
			eventInfo->AddAction(func);
			eventDic[eventName] = eventInfo;
		}
	}

	void EventCenter::RemoveEventListener(E_EventType eventName, const std::function<void()>& func)
	{
		if (eventDic.count(eventName)) {
			auto eventInfo = std::dynamic_pointer_cast<EventInfo<void>>(eventDic[eventName]);
			if (eventInfo) {
				auto it = std::find_if(eventInfo->actions.begin(), eventInfo->actions.end(),
									   [&func](const std::function<void()>& f)
									   {
										   return f.target_type() == func.target_type() && *f.target<void(*)()>() == *func.target<void(*)()>();
									   });
				if (it != eventInfo->actions.end()) {
					eventInfo->actions.erase(it);
				}
			}
		}
	}

	void EventCenter::ClearAll()
	{
		eventDic.clear();
	}

	void EventCenter::Clear(E_EventType eventName)
	{
		if (eventDic.count(eventName))
			eventDic.erase(eventName);
	}
}