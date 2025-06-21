#pragma once

#include "System.h"

class SystemFlicker : public ecs::System
{
public:
	SystemFlicker(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Flicker::_mType); // フリッカー効果コンポーネント
		requiredComponents.insert(Color::_mType);   // 色（アルファ値を制御）
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		Flicker& flicker = mWorld.getComponentStore<Flicker>().get(aEntity);
		Color& color = mWorld.getComponentStore<Color>().get(aEntity);

		if (!flicker.isActive) return; // 効果が無効の場合はスキップ

		// 経過時間を更新
		flicker.elapsedTime += aDeltaTime;

		if (flicker.isFadingIn)
		{
			// フェードイン中
			if (flicker.elapsedTime <= flicker.fadeInDuration)
			{
				flicker.currentAlpha = flicker.minAlpha +
					(flicker.maxAlpha - flicker.minAlpha) * (flicker.elapsedTime / flicker.fadeInDuration);
			}
			else
			{
				// フェードイン終了、フェードアウトに切り替え
				flicker.elapsedTime = 0.0f;      // 時間をリセット
				flicker.isFadingIn = false;      // フェードアウトに切り替え
			}
		}
		else
		{
			// フェードアウト中
			if (flicker.elapsedTime <= flicker.fadeOutDuration)
			{
				flicker.currentAlpha = flicker.maxAlpha -
					(flicker.maxAlpha - flicker.minAlpha) * (flicker.elapsedTime / flicker.fadeOutDuration);
			}
			else
			{
				// 効果終了、停止状態に戻す
				flicker.isActive = false;
				flicker.elapsedTime = 0.0f;
				flicker.currentAlpha = flicker.minAlpha; // 最小透明度にリセット
			}
		}

		// アルファ値を色に反映
		color.A = flicker.currentAlpha;
	}

	// フリッカー効果を開始
	void startFlicker(ecs::Entity aEntity) {
		auto& flicker = mWorld.getComponentStore<Flicker>().get(aEntity);
		flicker.isActive = true;
		flicker.isFadingIn = true;
		flicker.elapsedTime = 0.0f;
	}
};

