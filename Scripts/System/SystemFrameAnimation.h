#pragma once

#include "System.h"

class SystemFrameAnimation : public ecs::System
{
public:
	SystemFrameAnimation(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(FrameAnimation::_mType);
		requiredComponents.insert(Position::_mType); // アニメーションの描画には位置情報など他のコンポーネントも必要になる場合がある
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		FrameAnimation& animation = mWorld.getComponentStore<FrameAnimation>().get(aEntity);

		// アニメーションの経過時間を更新
		animation.elapsedTime += aDeltaTime;

		// フレームの持続時間を超えたら、次のフレームへ切り替え
		if (animation.elapsedTime >= animation.frameTime)
		{
			animation.elapsedTime = 0.0f; // 時間をリセット

			// 次のフレームへ
			animation.currentFrame++;

			// 最後のフレームに達した場合、ループするかチェック
			if (animation.currentFrame >= animation.frameIndices.size())
			{
				if (animation.loop)
				{
					animation.currentFrame = 0; // 最初のフレームに戻る
				}
				else
				{
					animation.currentFrame = animation.frameIndices.size() - 1; // 最後のフレームで止まる
				}
			}
		}
	}
};
