#pragma once

#include "System.h"

// ノートとプレイヤーの当たり判定を行い、トリガー処理を行うシステム
class SystemNoteTrigger : public ecs::System
{
public:
	SystemNoteTrigger(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(NoteTag::_mType);       // ノートタグ
		requiredComponents.insert(CleanState::_mType);    // 処理済み状態
		requiredComponents.insert(BoxCollider::_mType);   // 衝突判定用のボックス
		requiredComponents.insert(Position::_mType);      // 位置情報
		requiredComponents.insert(NoteType::_mType);      // ノートの種類
		requiredComponents.insert(Trigger::_mType);       // トリガーフラグ
		requiredComponents.insert(Deadline::_mType);      // 判定期限
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		CleanState& cleanState = mWorld.getComponentStore<CleanState>().get(aEntity);
		if (cleanState.isClean) return; // すでに処理済みならスキップ

		const BoxCollider& boxColl = mWorld.getComponentStore<BoxCollider>().get(aEntity);
		const Position& pos_n = mWorld.getComponentStore<Position>().get(aEntity);
		const NoteType& noteType = mWorld.getComponentStore<NoteType>().get(aEntity);
		Deadline& deadline = mWorld.getComponentStore<Deadline>().get(aEntity);
		Trigger& trigger = mWorld.getComponentStore<Trigger>().get(aEntity);

		// プレイヤーエンティティの一覧を取得
		const auto& playerStore = mWorld.getComponentStore<PlayerTag>().getComponents();

		for (auto& [player, playerTag] : playerStore)
		{
			const Position& pos_p = mWorld.getComponentStore<Position>().get(player);
			const BoxCollider& playerBoxColl = mWorld.getComponentStore<BoxCollider>().get(player);

			// プレイヤーの当たり判定ボックスを計算
			float playerLeft = pos_p.x + playerBoxColl.offsetX - playerBoxColl.width / 2;
			float playerRight = pos_p.x + playerBoxColl.offsetX + playerBoxColl.width / 2;
			float playerBottom = pos_p.y + playerBoxColl.offsetY - playerBoxColl.height / 2;
			float playerTop = pos_p.y + playerBoxColl.offsetY + playerBoxColl.height / 2;

			// ノートの当たり判定ボックスを計算
			float noteLeft = pos_n.x + boxColl.offsetX - boxColl.width / 2;
			float noteRight = pos_n.x + boxColl.offsetX + boxColl.width / 2;
			float noteBottom = pos_n.y + boxColl.offsetY - boxColl.height / 2;
			float noteTop = pos_n.y + boxColl.offsetY + boxColl.height / 2;

			// 衝突判定（AABB）
			if (playerRight > noteLeft && playerLeft < noteRight && playerTop > noteBottom && playerBottom < noteTop)
			{
				trigger.isTriggered = true;           // ノートがトリガーされたと記録
				deadline.value = 350.0f;              // 判定期限リセット

				// プレイヤーの子エンティティのエフェクトを有効化
				const Parent& children_player = mWorld.getComponentStore<Parent>().get(player);
				mWorld.getComponentStore<Flicker>().get(children_player.children.find("active_1")->second).isActive = true;
				mWorld.getComponentStore<Flicker>().get(children_player.children.find("active_2")->second).isActive = true;
				mWorld.getComponentStore<HeartBeat>().get(children_player.children.find("heart")->second).isActive = true;

				// 通常ノートの場合、ヘッドノートの判定期限も更新
				if (noteType.type != E_NoteType::DNA)
				{
					const Parent& children = mWorld.getComponentStore<Parent>().get(aEntity);
					Deadline& deadline_head = mWorld.getComponentStore<Deadline>().get(children.children.find("note_head")->second);
					deadline_head.value = 350.0f;

					break; // 一度当たったら他のプレイヤーはチェックしない
				}
			}
			else
			{
				trigger.isTriggered = false; // 当たっていなければトリガー解除
			}
		}
	}
};
