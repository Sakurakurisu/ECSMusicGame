#pragma once

#include "System.h"

ecs::Entity CreateTapNote(ecs::World& aWorld, float pos_x)
{
	Scale scale = Scale(550.0f, 100.0f);

	float pos_y = -540.0f - scale.y / 2;
	ecs::Entity note = aWorld.createEntity();

	ecs::Entity note_head = aWorld.createEntity();
	aWorld
		.addComponent(note_head, Position(pos_x, pos_y + scale.y / 2 - 20.0f))
		.addComponent(note_head, Scale(120.0f, 40.0f))
		.addComponent(note_head, Rotation(0.0f))
		.addComponent(note_head, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(note_head, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f }))
		.addComponent(note_head, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(note_head, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(note_head, Texture(note_headTex))
		.addComponent(note_head, Children(note))
		.addComponent(note_head, Speed(noteSpeed))
		.addComponent(note_head, Direction(0.0f, 1.0f))
		.addComponent(note_head, Deadline(Axis::Y, 640.0f))
		.registerEntity(note_head);

	ecs::Entity note_tail = aWorld.createEntity();
	aWorld
		.addComponent(note_tail, Position(pos_x, pos_y - scale.y / 2 + 20.0f))
		.addComponent(note_tail, Scale(120.0f, 40.0f))
		.addComponent(note_tail, Rotation(0.0f))
		.addComponent(note_tail, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(note_tail, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f }))
		.addComponent(note_tail, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(note_tail, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(note_tail, Texture(note_tailTex))
		.addComponent(note_tail, Children(note))
		.addComponent(note_tail, Speed(noteSpeed))
		.addComponent(note_tail, Direction(0.0f, 1.0f))
		.addComponent(note_tail, Deadline(Axis::Y, 640.0f))
		.registerEntity(note_tail);

	std::map <std::string, ecs::Entity> children;
	children.insert(std::pair<std::string, ecs::Entity>("note_head", note_head));
	children.insert(std::pair<std::string, ecs::Entity>("note_tail", note_tail));

	aWorld
		.addComponent(note, Position(pos_x, pos_y))
		.addComponent(note, std::move(scale))
		.addComponent(note, Rotation(0.0f))
		.addComponent(note, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(note, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f }))
		.addComponent(note, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(note, Indices({ 0, 1, 2,0, 2, 3 }))
		.addComponent(note, Direction(0.0f, 1.0f))
		.addComponent(note, Speed(noteSpeed))
		.addComponent(note, NoteType(Tap))
		.addComponent(note, BoxCollider(0.0f, 0.0f, 200.0f, 200.0f))
		.addComponent(note, Deadline(Axis::Y, 640.0f))
		.addComponent(note, CleanState())
		.addComponent(note, Trigger())
		.addComponent(note, Texture(holdNoteTex))
		.addComponent(note, Parent(children))
		.addComponent(note, NoteTag());

	return note;
}

ecs::Entity CreateHoldNote(ecs::World& aWorld, float pos_x, HoldState& holdState)
{
	Speed speed = Speed(noteSpeed);

	Scale scale = Scale(550.0f, speed.value * holdState.requiredHoldTime);

	float pos_y = -540.0f - scale.y / 2;
	ecs::Entity note = aWorld.createEntity();

	ecs::Entity note_head = aWorld.createEntity();
	aWorld
		.addComponent(note_head, Position(pos_x, pos_y + scale.y / 2 - 20.0f))
		.addComponent(note_head, Scale(120.0f, 40.0f))
		.addComponent(note_head, Rotation(0.0f))
		.addComponent(note_head, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(note_head, Vertex({ -0.5f, -0.5f, 0.0f,0.5f, -0.5f, 0.0f,0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(note_head, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(note_head, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(note_head, Texture(note_headTex))
		.addComponent(note_head, Children(note))
		.addComponent(note_head, Speed(noteSpeed))
		.addComponent(note_head, Direction(0.0f, 1.0f))
		.addComponent(note_head, Deadline(Axis::Y, 640.0f))
		.registerEntity(note_head);

	ecs::Entity note_tail = aWorld.createEntity();
	aWorld
		.addComponent(note_tail, Position(pos_x, pos_y - scale.y / 2 + 20.0f))
		.addComponent(note_tail, Scale(120.0f, 40.0f))
		.addComponent(note_tail, Rotation(0.0f))
		.addComponent(note_tail, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(note_tail, Vertex({ -0.5f, -0.5f, 0.0f,0.5f, -0.5f, 0.0f,0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(note_tail, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(note_tail, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(note_tail, Texture(note_tailTex))
		.addComponent(note_tail, Children(note))
		.addComponent(note_tail, Speed(noteSpeed))
		.addComponent(note_tail, Direction(0.0f, 1.0f))
		.addComponent(note_tail, Deadline(Axis::Y, 640.0f))
		.registerEntity(note_tail);

	std::map <std::string, ecs::Entity> children;
	children.insert(std::pair<std::string, ecs::Entity>("note_head", note_head));
	children.insert(std::pair<std::string, ecs::Entity>("note_tail", note_tail));

	aWorld
		.addComponent(note, Position(pos_x, pos_y))
		.addComponent(note, std::move(scale))
		.addComponent(note, Rotation(0.0f))
		.addComponent(note, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(note, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f }))
		.addComponent(note, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(note, Indices({ 0, 1, 2,0, 2, 3 }))
		.addComponent(note, Direction(0.0f, 1.0f))
		.addComponent(note, std::move(speed))
		.addComponent(note, NoteType(Hold))
		.addComponent(note, BoxCollider(0.0f, 0.0f, 200.0f, speed.value * holdState.requiredHoldTime))
		.addComponent(note, Deadline(Axis::Y, 640.0f))
		.addComponent(note, CleanState())
		.addComponent(note, Trigger())
		.addComponent(note, std::move(holdState))
		.addComponent(note, Texture(holdNoteTex))
		.addComponent(note, Parent(children))
		.addComponent(note, NoteTag());

	return note;
}

ecs::Entity CreateDNANote(ecs::World& aWorld, float pos_x, HoldState& holdState)
{
	Speed speed = Speed(noteSpeed);

	Scale scale = Scale(324.0f, speed.value * holdState.requiredHoldTime);
	//Scale scale = Scale(120.0f, 535.0f);
	float uv = holdState.requiredHoldTime / 10.0f;
	float pos_y = -540.0f - scale.y / 2;
	ecs::Entity note = aWorld.createEntity();
	aWorld
		.addComponent(note, Position(pos_x, pos_y))
		//.addComponent(note, Position(0.0f, 0.0f))
		.addComponent(note, std::move(scale))
		.addComponent(note, Rotation(0.0f))
		.addComponent(note, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(note, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(note, TexCoords({ 0.0f, uv, 1.0f, 0.0f, 1.0f, uv, 0.0f, 1.0f }))
		.addComponent(note, Indices({ 0, 1, 2,0, 2, 3 }))
		.addComponent(note, Direction(0.0f, 1.0f))
		.addComponent(note, std::move(speed))
		.addComponent(note, NoteType(DNA))
		.addComponent(note, BoxCollider(0.0f, 0.0f, 324.0f, speed.value * holdState.requiredHoldTime))
		.addComponent(note, Deadline(Axis::Y, 640.0f))
		.addComponent(note, CleanState())
		.addComponent(note, Trigger())
		.addComponent(note, std::move(holdState))
		.addComponent(note, Texture(dnaNoteTex))
		.addComponent(note, NoteTag());

	return note;
}


class SystemCreateNote : public ecs::System
{
public:
	SystemCreateNote(ecs::World& aManager) : ecs::System(aManager)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Timer::_mType);

		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		Timer& timer = mWorld.getComponentStore<Timer>().get(aEntity);

		while (!noteList.empty())
		{
			NoteData note = *noteList.begin();

			ecs::Entity noteEntity;
			if (timer.accumulatedTime > note.spawnTime)
			{
				noteEntity = mWorld.createEntity();
				if (note.type == E_NoteType::Tap)
				{
					noteEntity = CreateTapNote(mWorld, note.pos_x);
				}
				else if (note.type == E_NoteType::Hold)
				{
					HoldState holdState(note.holdDuration);
					noteEntity = CreateHoldNote(mWorld, note.pos_x, holdState);
				}
				else if (note.type == E_NoteType::DNA)
				{
					HoldState holdState(note.holdDuration);
					noteEntity = CreateDNANote(mWorld, note.pos_x, holdState);
				}

				noteList.erase(noteList.begin());

				mWorld.registerEntity(noteEntity);
			}
			else
			{
				break;
			}
		}
	}
};