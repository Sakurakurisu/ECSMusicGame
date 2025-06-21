#pragma once

#include "Component.h"

// 点滅エフェクト
struct Flicker : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float fadeInDuration;
	float fadeOutDuration;
	float minAlpha;
	float maxAlpha;
	float currentAlpha;
	bool isActive;
	bool isFadingIn;
	float elapsedTime;

	Flicker(float aFadeInDuration, float aFadeOutDuration, float aMinAlpha = 0.0f, float aMaxAlpha = 1.0f)
		: fadeInDuration(aFadeInDuration), fadeOutDuration(aFadeOutDuration),
		minAlpha(aMinAlpha), maxAlpha(aMaxAlpha), currentAlpha(aMinAlpha),
		isActive(false), isFadingIn(true), elapsedTime(0.0f) {}
};