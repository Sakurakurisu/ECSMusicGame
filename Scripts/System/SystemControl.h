#pragma once

#include "System.h"

class SystemControl : public ecs::System
{
public:
	SystemControl(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		// Input と Direction コンポーネントが必要
		requiredComponents.insert(Input::_mType);
		requiredComponents.insert(Direction::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		const Control& control = mWorld.getComponentStore<Control>().get(aEntity);
		Input& input = mWorld.getComponentStore<Input>().get(aEntity);
		Direction& dir = mWorld.getComponentStore<Direction>().get(aEntity);

		dir.vx = 0.0f;
		dir.vy = 0.0f;

		// 方向の処理用ラムダ（押されていて、逆方向が押されていない場合に有効）
		auto processDirection = [&](E_KeyCode primaryKey, E_KeyCode oppositeKey, float vx, float vy)
			{
				if (input.keyStates[primaryKey] && !input.keyStates[oppositeKey])
				{
					dir.vx = vx;
					dir.vy = vy;
				}
			};

		for (auto keyCode = control.activeKeyCodes.begin(); keyCode != control.activeKeyCodes.end(); ++keyCode)
		{
			switch (*keyCode)
			{
				case E_KeyCode::keyCodeDown_W:
					processDirection(E_KeyCode::keyCodeDown_W, E_KeyCode::keyCodeDown_S, 0.0f, 1.0f);
					break;
				case E_KeyCode::keyCodeDown_Up:
					processDirection(E_KeyCode::keyCodeDown_Up, E_KeyCode::keyCodeDown_Down, 0.0f, 1.0f);
					break;
				case E_KeyCode::keyCodeDown_A:
					processDirection(E_KeyCode::keyCodeDown_A, E_KeyCode::keyCodeDown_D, -1.0f, 0.0f);
					break;
				case E_KeyCode::keyCodeDown_LEFT:
					processDirection(E_KeyCode::keyCodeDown_LEFT, E_KeyCode::keyCodeDown_RIGHT, -1.0f, 0.0f);
					break;
				case E_KeyCode::keyCodeDown_S:
					processDirection(E_KeyCode::keyCodeDown_S, E_KeyCode::keyCodeDown_W, 0.0f, -1.0f);
					break;
				case E_KeyCode::keyCodeDown_Down:
					processDirection(E_KeyCode::keyCodeDown_Down, E_KeyCode::keyCodeDown_Up, 0.0f, -1.0f);
					break;
				case E_KeyCode::keyCodeDown_D:
					processDirection(E_KeyCode::keyCodeDown_D, E_KeyCode::keyCodeDown_A, 1.0f, 0.0f);
					break;
				case E_KeyCode::keyCodeDown_RIGHT:
					processDirection(E_KeyCode::keyCodeDown_RIGHT, E_KeyCode::keyCodeDown_LEFT, 1.0f, 0.0f);
					break;

					// Eキー：位置と当たり判定エリアを左に移動（トグル）
				case E_KeyCode::keyCodeDown_E:
					if (input.keyStates[E_KeyCode::keyCodeDown_E])
					{
						if (!wasPressedE) // 前回押されていない場合のみ実行
						{
							mWorld.getComponentStore<Position>().get(aEntity).x = -100.0f;
							mWorld.getComponentStore<Area>().get(aEntity).right = -75.0f;
							mWorld.getComponentStore<Area>().get(aEntity).left = -125.0f;
							wasPressedE = true; // 押下フラグを更新
						}
					}
					else
					{
						if (wasPressedE) // 前回押されていた場合のみ実行
						{
							mWorld.getComponentStore<Area>().get(aEntity).right = -325.0f;
							mWorld.getComponentStore<Area>().get(aEntity).left = -785.0f;
							mWorld.getComponentStore<Position>().get(aEntity).x = -555.0f;
							wasPressedE = false; // フラグをリセット
						}
					}
					break;

					// Rキー：位置と当たり判定エリアを右に移動（トグル）
				case E_KeyCode::keyCodeDown_R:
					if (input.keyStates[E_KeyCode::keyCodeDown_R])
					{
						if (!wasPressedR)
						{
							mWorld.getComponentStore<Position>().get(aEntity).x = 100.0f;
							mWorld.getComponentStore<Area>().get(aEntity).left = 75.0f;
							mWorld.getComponentStore<Area>().get(aEntity).right = 125.0f;
							wasPressedR = true;
						}
					}
					else
					{
						if (wasPressedR)
						{
							mWorld.getComponentStore<Area>().get(aEntity).left = 325.0f;
							mWorld.getComponentStore<Area>().get(aEntity).right = 785.0f;
							mWorld.getComponentStore<Position>().get(aEntity).x = 555.0f;
							wasPressedR = false;
						}
					}
					break;

				default:
					break;
			}
		}
	}

private:
	bool wasPressedE = false;  // Eキーの前回状態を記録
	bool wasPressedR = false;  // Rキーの前回状態を記録
};