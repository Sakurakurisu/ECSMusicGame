#pragma once

#include "Component.h"
#include <unordered_map>

// 入力キー定義
enum class E_KeyCode
{
	keyCodeDown_W,
	keyCodeDown_A,
	keyCodeDown_S,
	keyCodeDown_D,
	keyCodeDown_Up,
	keyCodeDown_LEFT,
	keyCodeDown_Down,
	keyCodeDown_RIGHT,
	keyCodeDown_E,
	keyCodeDown_R
};

// 入力状態
struct Input : public ecs::Component
{
	static const ecs::ComponentType _mType;

	std::unordered_map<E_KeyCode, bool> keyStates; // 各キーの状態

	Input()
	{
		keyStates[E_KeyCode::keyCodeDown_W] = false;
		keyStates[E_KeyCode::keyCodeDown_A] = false;
		keyStates[E_KeyCode::keyCodeDown_S] = false;
		keyStates[E_KeyCode::keyCodeDown_D] = false;
		keyStates[E_KeyCode::keyCodeDown_Up] = false;
		keyStates[E_KeyCode::keyCodeDown_LEFT] = false;
		keyStates[E_KeyCode::keyCodeDown_Down] = false;
		keyStates[E_KeyCode::keyCodeDown_RIGHT] = false;
		keyStates[E_KeyCode::keyCodeDown_E] = false;
		keyStates[E_KeyCode::keyCodeDown_R] = false;
	}
};