#pragma once

#include "System.h"

class SystemOrbit : public ecs::System
{
public:
	SystemOrbit(ecs::World& world) : ecs::System(world) 
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Position::_mType);   // 位置情報
		requiredComponents.insert(Rotation::_mType);   // 回転角
		requiredComponents.insert(Orbit::_mType);      // 軌道回転情報
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		Position& position = mWorld.getComponentStore<Position>().get(aEntity);
		Rotation& rotation = mWorld.getComponentStore<Rotation>().get(aEntity);
		Orbit& orbit = mWorld.getComponentStore<Orbit>().get(aEntity);

		if (!orbit.isActive)
			return; // アクティブでない場合はスキップ

		// 回転角を更新
		rotation.angle -= orbit.orbitSpeed * aDeltaTime;

		// 角度が360度を超えた場合の処理
		if (rotation.angle >= 360.0f)
		{
			if (orbit.isLooping)
			{
				rotation.angle = fmod(rotation.angle, 360.0f); // ループモード：0〜360に巻き戻し
			}
			else
			{
				rotation.angle = 360.0f;  // 非ループモード：360度で停止
				orbit.isActive = false;   // 回転停止
			}
		}

		// 角度をラジアンに変換（GLM未使用）
		const float pi = 3.14159265358979323846f;
		float radians = rotation.angle * (pi / 180.0f); // 度 → ラジアン

		// 軌道上の新しい位置を計算
		position.x = orbit.offsetX + cos(radians);
		position.y = orbit.offsetY + sin(radians);
	}
};
