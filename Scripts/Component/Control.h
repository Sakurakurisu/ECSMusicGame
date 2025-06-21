#pragma once

#include "Component.h"
#include <vector>
#include "Input.h"

// 操作対象のキー
struct Control : public ecs::Component
{
	static const ecs::ComponentType _mType;

	std::vector<E_KeyCode> activeKeyCodes;

	Control(std::vector<E_KeyCode> aActiveKeyCodes) : activeKeyCodes(aActiveKeyCodes) {}
};