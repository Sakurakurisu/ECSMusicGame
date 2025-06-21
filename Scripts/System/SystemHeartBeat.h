#pragma once

#include "System.h"

class SystemHeartBeat : public ecs::System
{
public:
	SystemHeartBeat(ecs::World& world) : ecs::System(world) 
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Scale::_mType);       // スケール情報
		requiredComponents.insert(HeartBeat::_mType);   // 心拍エフェクト情報
		requiredComponents.insert(TexCoords::_mType);   // テクスチャのUV座標
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		Scale& scale = mWorld.getComponentStore<Scale>().get(aEntity);
		HeartBeat& heartBeat = mWorld.getComponentStore<HeartBeat>().get(aEntity);
		TexCoords& texCoords = mWorld.getComponentStore<TexCoords>().get(aEntity);

		// UVのオフセットを更新（横方向スクロール）
		texCoords.uvOffset += heartBeat.scrollSpeed * aDeltaTime;

		// オフセットが1.0を超えたらリセット（ループ効果）
		if (texCoords.uvOffset > 1.0f)
			texCoords.uvOffset -= 1.0f;

		// UV座標を更新して右に流れるように設定
		texCoords.coords =
		{
			0.0f + texCoords.uvOffset, 0.0f,
			1.0f + texCoords.uvOffset, 0.0f,
			1.0f + texCoords.uvOffset, 1.0f,
			0.0f + texCoords.uvOffset, 1.0f
		};

		// 一時停止中かどうかを確認
		if (heartBeat.isPaused)
		{
			heartBeat.currentPauseTime += aDeltaTime;
			if (heartBeat.currentPauseTime >= heartBeat.pauseDuration)
			{
				heartBeat.isPaused = false;           // 停止終了、心拍再開
				heartBeat.currentPauseTime = 0.0f;    // タイマーリセット
			}
			return;  // 停止中のためスケール処理をスキップ
		}

		if (!heartBeat.isActive)
			return;

		// 拡大／縮小処理（一定速度で変化）
		float growthAmount = heartBeat.growthRate * aDeltaTime; // 拡縮量

		if (heartBeat.growing)
		{
			// 拡大処理
			scale.y += growthAmount;

			// 最大値を超えた場合、縮小へ切り替え
			if (scale.y >= heartBeat.maxScaleY)
			{
				scale.y = heartBeat.maxScaleY;
				heartBeat.growing = false;  // 縮小モードへ
			}
		}
		else
		{
			// 縮小処理
			scale.y -= growthAmount;

			// 最小値まで縮小したら、再び拡大へ切り替え
			if (scale.y <= 10.0f)
			{  // 最小スケール（ゼロ回避）
				scale.y = 10.0f;
				heartBeat.growing = true;   // 拡大モードへ
				heartBeat.isPaused = true;  // 一時停止開始
				heartBeat.isActive = false; // 心拍停止
			}
		}
	}
};
