#pragma once

#include "System.h"

class SystemShortenNote : public ecs::System
{
public:
	SystemShortenNote(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(NoteTag::_mType);         // ノートタグ
		requiredComponents.insert(Position::_mType);        // 位置
		requiredComponents.insert(Scale::_mType);           // スケール
		requiredComponents.insert(Deadline::_mType);        // 判定ライン
		requiredComponents.insert(BoxCollider::_mType);     // コライダー
		requiredComponents.insert(TexCoords::_mType);       // UV座標
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		Position& pos = mWorld.getComponentStore<Position>().get(aEntity);
		Scale& scale = mWorld.getComponentStore<Scale>().get(aEntity);
		const Deadline& deadline = mWorld.getComponentStore<Deadline>().get(aEntity);
		BoxCollider& boxColl = mWorld.getComponentStore<BoxCollider>().get(aEntity);
		TexCoords& texCoords = mWorld.getComponentStore<TexCoords>().get(aEntity);

		float minScaleY = 0.1f; // スケールがゼロにならないようにするための最小値

		// ノートの上端が判定ラインに近づいたか、超えたかを確認
		float noteTopY = pos.y + scale.y / 2.0f;

		if (noteTopY >= deadline.value)
		{
			// 短縮量を計算
			float shortenAmount = noteTopY - deadline.value;

			// スケールが最小値を下回らないよう調整
			if (scale.y - shortenAmount < minScaleY)
			{
				shortenAmount = scale.y - minScaleY;
			}

			// ノートの高さを短縮
			scale.y -= shortenAmount;

			// ノートの上端を維持するために位置を下に移動
			pos.y -= shortenAmount / 2.0f;

			// コライダーのYオフセットを調整（下端を合わせる）
			boxColl.offsetY -= shortenAmount / 2.0f;

			// UV座標を調整
			float uvAdjustment = scale.y / (scale.y + shortenAmount); // UVの補正率

			// UVの上端（インデックス5と7）を補正（順番：左下、右下、右上、左上）
			texCoords.coords[5] *= uvAdjustment;
			texCoords.coords[7] *= uvAdjustment;
		}
	}
};
