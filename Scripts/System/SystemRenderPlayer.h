#pragma once

#include "System.h"

class SystemRenderPlayer : public ecs::System
{
public:
	SystemRenderPlayer(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Position::_mType);
		requiredComponents.insert(Scale::_mType);
		requiredComponents.insert(Rotation::_mType);
		requiredComponents.insert(Color::_mType);
		requiredComponents.insert(Vertex::_mType);
		requiredComponents.insert(Indices::_mType);
		requiredComponents.insert(Texture::_mType);
		requiredComponents.insert(TexCoords::_mType);
		requiredComponents.insert(PlayerTag::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		const Position& pos = mWorld.getComponentStore<Position>().get(aEntity);
		const Scale& scale = mWorld.getComponentStore<Scale>().get(aEntity);
		const Rotation& rotation = mWorld.getComponentStore<Rotation>().get(aEntity);
		const Color& color = mWorld.getComponentStore<Color>().get(aEntity);
		const Vertex& vectex = mWorld.getComponentStore<Vertex>().get(aEntity);
		const Indices& indices = mWorld.getComponentStore<Indices>().get(aEntity);
		const Texture& texture = mWorld.getComponentStore<Texture>().get(aEntity);
		const TexCoords& texCoords = mWorld.getComponentStore<TexCoords>().get(aEntity);

		if (texture.ID != 0)
		{
			//Renderer::Instance().RenderShape(Float3(pos.x, pos.y, 0.0f),
			//								 Float3(scale.x, scale.y, 1.0f),
			//								 rotation.angle,
			//								 Float4(color.R, color.G, color.B, color.A),
			//								 vectex.vertices,
			//								 indices.indices,
			//								 texCoords.coords,
			//								 texture.ID);
		}
		Renderer::Instance().DrawSpriteQuad(pos.x, pos.y,
											scale.x, scale.y,
											Float4(color.R, color.G, color.B, color.A),
											texCoords.coords[0],
											texCoords.coords[1],
											texCoords.coords[2],
											texCoords.coords[5],
											texture.ID,
											rotation.angle);
	}
};