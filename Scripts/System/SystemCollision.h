#pragma once

#include "System.h"
#include "Position.h"
#include "BoxCollider.h"
#include "Area.h"
#include "World.h"

// 衝突判定システム
class SystemCollision : public ecs::System
{
public:
	SystemCollision(ecs::World& aManager) : ecs::System(aManager)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Position::_mType);      // 位置コンポーネント
		requiredComponents.insert(BoxCollider::_mType);   // 矩形コライダー
		requiredComponents.insert(Area::_mType);          // 当たり判定領域
		setRequiredComponents(requiredComponents);
	}

	// エンティティの衝突判定と位置補正
	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		Position& pos = mWorld.getComponentStore<Position>().get(aEntity);
		const BoxCollider& boxColl = mWorld.getComponentStore<BoxCollider>().get(aEntity);
		const Area& area = mWorld.getComponentStore<Area>().get(aEntity);

		// エンティティの左右端を計算
		float entityRight = pos.x + boxColl.offsetX + boxColl.width / 2;
		float entityLeft = pos.x + boxColl.offsetX - boxColl.width / 2;

		// 右端がエリアを超えた場合、位置を補正
		if (entityRight >= area.right)
		{
			pos.x -= (entityRight - area.right);
		}
		// 左端がエリアを超えた場合、位置を補正
		else if (entityLeft <= area.left)
		{
			pos.x += (area.left - entityLeft);
		}

		// エンティティの上下端を計算
		float entityTop = pos.y + boxColl.offsetY + boxColl.height / 2;
		float entityBottom = pos.y + boxColl.offsetY - boxColl.height / 2;

		// 上端がエリアを超えた場合、位置を補正
		if (entityTop >= area.top)
		{
			pos.y -= (entityTop - area.top);
		}
		// 下端がエリアを超えた場合、位置を補正
		else if (entityBottom <= area.bottom)
		{
			pos.y += (area.bottom - entityBottom);
		}
	}
};
