#pragma once

#include "System.h"

class SystemAudio : public ecs::System
{
public:
	SystemAudio(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		AudioComponent& audio = mWorld.getComponentStore<AudioComponent>().get(aEntity);

		if (audio.isPlaying)
		{
			// 音声が再生中の場合、SDL や OpenAL などのライブラリを使って音を再生することができる
			// 実際の再生処理は AudioEngine クラスなどで行うと仮定
			//AudioEngine::Play(audio.filePath, audio.volume, audio.loop);
		}
		else
		{
			// 再生を停止
			//AudioEngine::Stop(audio.filePath);
		}
	}
};
