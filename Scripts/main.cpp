#include <iostream>
#include <chrono>
#include <thread>
#include "Renderer.h"
#include "Component.h"
#include "ComponentStore.h"
#include "World.h"
#include "AudioManager.h"
#include <random>
#include <fstream>
#include <sstream>


#define L1 -760.0f
//#define L2 -100.0f
//#define R1 100.0f
#define L2 -350.0f
#define R1 350.0f
#define R2 760.0f
#define C 0.0f
#define noteSpeed 1000.0f

GLint playerTex_1;
GLint heartbeatTex;
GLint deadlineTex;
GLint holdNoteTex;
GLint lightTex;
GLint lineTex;
GLint snowTex;
GLint activeTex_1;
GLint activeTex_2;
GLint note_headTex;
GLint note_tailTex;
GLint dnaNoteTex;
GLint endTex;
GLint titleTex;
GLint coverTex;

ecs::Entity activeLight_L1;
ecs::Entity activeLight_L2;
ecs::Entity activeLight_R1;
ecs::Entity activeLight_R2;
ecs::Entity activeLight_C1;
ecs::Entity activeLight_C2;

ecs::Entity activeLine_L1;
ecs::Entity activeLine_L2;
ecs::Entity activeLine_R1;
ecs::Entity activeLine_R2;
ecs::Entity activeLine_C1;
ecs::Entity activeLine_C2;

ecs::Entity activeSnow_L1;
ecs::Entity activeSnow_L2;
ecs::Entity activeSnow_R1;
ecs::Entity activeSnow_R2;
ecs::Entity activeSnow_C1;
ecs::Entity activeSnow_C2;

ecs::Entity player_L;
ecs::Entity player_R;

enum E_NoteType
{
	Tap,
	Hold,
	DNA
};

struct NoteData
{
	float pos_x;
	E_NoteType type;
	float spawnTime;
	float holdDuration;

	NoteData(float aPos_x, E_NoteType aType, float aSpawnTime, float aHoldDuration)
		: pos_x(aPos_x), type(aType), spawnTime(aSpawnTime), holdDuration(aHoldDuration) { }
};
std::vector<NoteData> noteList;

void InitTexture()
{
	playerTex_1 = Renderer::Instance().LoadTexture("assets/art/player_1.png");
	heartbeatTex = Renderer::Instance().LoadTexture("assets/art/heart.png");
	deadlineTex = Renderer::Instance().LoadTexture("assets/art/deadline.png");
	holdNoteTex = Renderer::Instance().LoadTexture("assets/art/hold_note.png");
	lightTex = Renderer::Instance().LoadTexture("assets/art/light.png");
	lineTex = Renderer::Instance().LoadTexture("assets/art/line.png");
	snowTex = Renderer::Instance().LoadTexture("assets/art/snow.png");
	activeTex_1 = Renderer::Instance().LoadTexture("assets/art/active_1.png");
	activeTex_2 = Renderer::Instance().LoadTexture("assets/art/active_2.png");
	note_headTex = Renderer::Instance().LoadTexture("assets/art/note_head.png");
	note_tailTex = Renderer::Instance().LoadTexture("assets/art/note_tail.png");
	dnaNoteTex = Renderer::Instance().LoadTexture("assets/art/DNA_note.png");
	endTex = Renderer::Instance().LoadTexture("assets/art/end.png");
	titleTex = Renderer::Instance().LoadTexture("assets/art/title.png");
	coverTex = Renderer::Instance().LoadTexture("assets/art/cover.png");
}

#pragma region コンポーネント
#include "Position.h"
#include "Scale.h"
#include "Rotation.h"
#include "Color.h"
#include "Vertex.h"
#include "Indices.h"
#include "Direction.h"
#include "Speed.h"
#include "Input.h"
#include "Control.h"
#include "BoxCollider.h"
#include "Area.h"
#include "NoteType.h"
#include "SpawnInterval.h"
#include "Timer.h"
#include "Deadline.h"
#include "CleanState.h"
#include "Trigger.h"
#include "PlayerTag.h"
#include "NoteTag.h"
#include "AudioComponent.h"
#include "HoldState.h"
#include "FrameAnimation.h"
#include "Texture.h"
#include "Children.h"
#include "RelativePosition.h"
#include "Parent.h"
#include "Flicker.h"
#include "TexCoords.h"
#include "HeartBeat.h"
#include "Orbit.h"

#pragma endregion コンポーネント

const ecs::ComponentType Position::_mType = 1;
const ecs::ComponentType Scale::_mType = 2;
const ecs::ComponentType Rotation::_mType = 3;
const ecs::ComponentType Color::_mType = 4;
const ecs::ComponentType Vertex::_mType = 5;
const ecs::ComponentType Indices::_mType = 6;
const ecs::ComponentType Direction::_mType = 7;
const ecs::ComponentType Speed::_mType = 8;
const ecs::ComponentType Input::_mType = 9;
const ecs::ComponentType Control::_mType = 10;
const ecs::ComponentType BoxCollider::_mType = 11;
const ecs::ComponentType Area::_mType = 12;
const ecs::ComponentType NoteType::_mType = 13;
const ecs::ComponentType SpawnInterval::_mType = 14;
const ecs::ComponentType Timer::_mType = 15;
const ecs::ComponentType Deadline::_mType = 16;
const ecs::ComponentType CleanState::_mType = 17;
const ecs::ComponentType Trigger::_mType = 18;
const ecs::ComponentType PlayerTag::_mType = 19;
const ecs::ComponentType NoteTag::_mType = 20;
const ecs::ComponentType AudioComponent::_mType = 21;
const ecs::ComponentType HoldState::_mType = 22;
const ecs::ComponentType FrameAnimation::_mType = 23;
const ecs::ComponentType Texture::_mType = 24;
const ecs::ComponentType Children::_mType = 25;
const ecs::ComponentType RelativePosition::_mType = 26;
const ecs::ComponentType Parent::_mType = 27;
const ecs::ComponentType Flicker::_mType = 28;
const ecs::ComponentType TexCoords::_mType = 29;
const ecs::ComponentType HeartBeat::_mType = 30;
const ecs::ComponentType Orbit::_mType = 31;

void CreateComponentStore(ecs::World& world)
{
	world
		.createComponentStore<Position>()
		.createComponentStore<Scale>()
		.createComponentStore<Rotation>()
		.createComponentStore<Color>()
		.createComponentStore<Vertex>()
		.createComponentStore<Indices>()
		.createComponentStore<Direction>()
		.createComponentStore<Speed>()
		.createComponentStore<Input>()
		.createComponentStore<Control>()
		.createComponentStore<BoxCollider>()
		.createComponentStore<Area>()
		.createComponentStore<NoteType>()
		.createComponentStore<SpawnInterval>()
		.createComponentStore<Timer>()
		.createComponentStore<Deadline>()
		.createComponentStore<CleanState>()
		.createComponentStore<Trigger>()
		.createComponentStore<PlayerTag>()
		.createComponentStore<NoteTag>()
		.createComponentStore<AudioComponent>()
		.createComponentStore<HoldState>()
		.createComponentStore<FrameAnimation>()
		.createComponentStore<Texture>()
		.createComponentStore<Children>()
		.createComponentStore<RelativePosition>()
		.createComponentStore<Parent>()
		.createComponentStore<Flicker>()
		.createComponentStore<TexCoords>()
		.createComponentStore<HeartBeat>()
		.createComponentStore<Orbit>();
}

#pragma region システム

#include "SystemRenderPlayer.h"
#include "SystemRenderShape.h"
#include "SystemMove.h"
#include "SystemInput.h"
#include "SystemControl.h"
#include "SystemCollision.h"
#include "SystemTimeAccumulator.h"
#include "SystemDeadline.h"
//#include "SystemNoteClean.h"
#include "SystemNoteTrigger.h"
#include "SystemNoteTap.h"
#include "SystemNoteHold.h"
#include "SystemShortenNote.h"
#include "SystemAudio.h"
#include "SystemFrameAnimation.h"
#include "SystemActiveNote.h"
#include "SystemHierarchyDir.h"
#include "SystemHierarchyMove.h"
#include "SystemFlicker.h"
#include "SystemHeartBeat.h"
#include "SystemOrbit.h"
#include "SystemCreateNote.h"


#pragma endregion システム

void AddSystem(ecs::World& aWorld)
{
	aWorld
		.addSystem(ecs::System::Ptr(new SystemTimeAccumulator(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemCreateNote(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemInput(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemControl(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemHierarchyDir(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemMove(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemCollision(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemHeartBeat(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemHierarchyMove(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemNoteTrigger(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemActiveNote(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemNoteTap(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemNoteHold(aWorld)))
		//.addSystem(ecs::System::Ptr(new SystemNoteClean(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemShortenNote(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemDeadline(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemFlicker(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemRenderShape(aWorld)))
		.addSystem(ecs::System::Ptr(new SystemRenderPlayer(aWorld)));
}

ecs::Entity CreateLight(ecs::World& aWorld, float pos_x)
{
	ecs::Entity entity = aWorld.createEntity();
	aWorld
		.addComponent(entity, Position(pos_x, 150.0f))
		.addComponent(entity, Scale(582.0f * 0.35, 2240.0f * 0.25))
		.addComponent(entity, Rotation(0.0f))
		.addComponent(entity, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(entity, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(entity, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(entity, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(entity, Texture(lightTex));

	return entity;
}

ecs::Entity CreateLine(ecs::World& aWorld, float pos_x)
{
	ecs::Entity entity = aWorld.createEntity();
	aWorld
		.addComponent(entity, Position(pos_x, 100.0f))
		.addComponent(entity, Scale(340.0f * 0.35, 1798.0f * 0.25))
		.addComponent(entity, Rotation(0.0f))
		.addComponent(entity, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(entity, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(entity, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(entity, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(entity, Texture(lineTex));

	return entity;
}

ecs::Entity CreateSnow(ecs::World& aWorld, float pos_x)
{
	ecs::Entity entity = aWorld.createEntity();
	aWorld
		.addComponent(entity, Position(pos_x, 100.0f))
		.addComponent(entity, Scale(447.0f * 0.35f, 1314.0f * 0.25f))
		.addComponent(entity, Rotation(0.0f))
		.addComponent(entity, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(entity, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(entity, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(entity, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(entity, Texture(snowTex));

	return entity;
}

ecs::Entity CreateActive_1(ecs::World& aWorld, ecs::Entity parent)
{
	ecs::Entity active_1 = aWorld.createEntity();
	aWorld
		.addComponent(active_1, Position(0.0f, 0.0f))
		.addComponent(active_1, Scale(936.0f, 612.0f, 0.4))
		.addComponent(active_1, Rotation(0.0f))
		.addComponent(active_1, Color(1.0f, 1.0f, 1.0f, 0.0f))
		.addComponent(active_1, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(active_1, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(active_1, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(active_1, Children(parent))
		.addComponent(active_1, RelativePosition(0.0f, 0.0f))
		.addComponent(active_1, Flicker(0.5, 1.0))
		.addComponent(active_1, Texture(activeTex_1));
	aWorld.registerEntity(active_1);
	return active_1;
}

ecs::Entity CreateActive_2(ecs::World& aWorld, ecs::Entity parent)
{
	ecs::Entity active_2 = aWorld.createEntity();
	aWorld
		.addComponent(active_2, Position(0.0f, 0.0f))
		.addComponent(active_2, Scale(888.0f, 1048.0f, 0.4))
		.addComponent(active_2, Rotation(0.0f))
		.addComponent(active_2, Color(1.0f, 1.0f, 1.0f, 0.0f))
		.addComponent(active_2, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(active_2, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(active_2, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(active_2, Children(parent))
		.addComponent(active_2, RelativePosition(0.0f, -75.0f))
		.addComponent(active_2, Flicker(0.5, 1.0))
		.addComponent(active_2, Texture(activeTex_2));
	aWorld.registerEntity(active_2);
	return active_2;
}

void CreateEntity(ecs::World& aWorld)
{
#pragma region Entity：deadline
	ecs::Entity deadline = aWorld.createEntity();
	aWorld
		.addComponent(deadline, Position(0.0f, -136.0f))
		.addComponent(deadline, Scale(2480.0f, 1052.0f))
		.addComponent(deadline, Rotation(0.0f))
		.addComponent(deadline, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(deadline, Vertex({ -0.5f, -0.5f, 0.0f,0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(deadline, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(deadline, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(deadline, Texture(deadlineTex));
#pragma endregion

#pragma region Entity：player_L
	player_L = aWorld.createEntity();
	ecs::Entity activeL_1 = CreateActive_1(aWorld, player_L);
	ecs::Entity activeL_2 = CreateActive_2(aWorld, player_L);
	ecs::Entity heart_L = aWorld.createEntity();
	std::map<std::string, ecs::Entity> children_L;
	children_L.insert(std::make_pair("active_1", activeL_1));
	children_L.insert(std::make_pair("active_2", activeL_2));
	children_L.insert(std::make_pair("heart", heart_L));
	aWorld
		.addComponent(player_L, Position(-500.0f, 350.0f))
		.addComponent(player_L, Scale(779.0f, 740.0f, 0.4))
		.addComponent(player_L, Rotation(0.0f))
		.addComponent(player_L, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(player_L, Vertex({ -0.5f, -0.5f, 0.0f,0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(player_L, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(player_L, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(player_L, Direction(0.0f, 0.0f))
		.addComponent(player_L, Speed(5000.0f))
		.addComponent(player_L, Input())
		.addComponent(player_L, Control({ E_KeyCode::keyCodeDown_A, E_KeyCode::keyCodeDown_D, E_KeyCode::keyCodeDown_E }))
		.addComponent(player_L, BoxCollider(0.0f, 0.0f, 50.0f, 50.0f))
		.addComponent(player_L, Area(-785.0f, -325.0f, 540, -540.0f))
		.addComponent(player_L, Texture(playerTex_1))
		.addComponent(player_L, Parent(children_L))
		.addComponent(player_L, PlayerTag());

#pragma endregion

#pragma region Entity：player_R

	player_R = aWorld.createEntity();
	ecs::Entity activeR_1 = CreateActive_1(aWorld, player_R);
	ecs::Entity activeR_2 = CreateActive_2(aWorld, player_R);
	ecs::Entity heart_R = aWorld.createEntity();
	std::map<std::string, ecs::Entity> children_R;
	children_R.insert(std::make_pair("active_1", activeR_1));
	children_R.insert(std::make_pair("active_2", activeR_2));
	children_R.insert(std::make_pair("heart", heart_R));
	aWorld
		.addComponent(player_R, Position(500.0f, 350.0f))
		.addComponent(player_R, Scale(779.0f, 740.0f, 0.4))
		.addComponent(player_R, Rotation(0.0f))
		.addComponent(player_R, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(player_R, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(player_R, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(player_R, Indices({ 0, 1, 2,0, 2, 3 }))
		.addComponent(player_R, Direction(0.0f, 0.0f))
		.addComponent(player_R, Speed(5000.0f))
		.addComponent(player_R, Input())
		.addComponent(player_R, Control({ E_KeyCode::keyCodeDown_LEFT, E_KeyCode::keyCodeDown_RIGHT, E_KeyCode::keyCodeDown_R }))
		.addComponent(player_R, BoxCollider(0.0f, 0.0f, 50.0f, 50.0f))
		.addComponent(player_R, Area(325.0f, 785.0f, 540, -540.0f))
		.addComponent(player_R, Texture(playerTex_1))
		.addComponent(player_R, Parent(children_R))
		.addComponent(player_R, PlayerTag());
#pragma endregion

	aWorld
		.addComponent(heart_L, Position(0.0f, 0.0f))
		.addComponent(heart_L, Scale(328.0f * 0.2f, 10.0f))
		.addComponent(heart_L, Rotation(0.0f))
		.addComponent(heart_L, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(heart_L, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(heart_L, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(heart_L, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(heart_L, HeartBeat(180.0f * 6, 105.2f, 0.49f, 0.2))
		.addComponent(heart_L, RelativePosition())
		.addComponent(heart_L, Children(player_L))
		.addComponent(heart_L, Texture(heartbeatTex));

	aWorld
		.addComponent(heart_R, Position(0.0f, 0.0f))
		.addComponent(heart_R, Scale(328.0f * 0.2f, 10.0f))
		.addComponent(heart_R, Rotation(0.0f))
		.addComponent(heart_R, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(heart_R, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(heart_R, TexCoords({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }))
		.addComponent(heart_R, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(heart_R, HeartBeat(180.0f * 6, 105.2f, 0.49f, 0.2))
		.addComponent(heart_R, RelativePosition())
		.addComponent(heart_R, Children(player_R))
		.addComponent(heart_R, Texture(heartbeatTex));

	activeLight_L1 = CreateLight(aWorld, L1);
	activeLight_L2 = CreateLight(aWorld, L2);
	activeLight_R1 = CreateLight(aWorld, R1);
	activeLight_R2 = CreateLight(aWorld, R2);
	activeLight_C1 = CreateLight(aWorld, -100);
	activeLight_C2 = CreateLight(aWorld, 100);

	activeLine_L1 = CreateLine(aWorld, L1);
	activeLine_L2 = CreateLine(aWorld, L2);
	activeLine_R1 = CreateLine(aWorld, R1);
	activeLine_R2 = CreateLine(aWorld, R2);
	activeLine_C1 = CreateLine(aWorld, -100);
	activeLine_C2 = CreateLine(aWorld, 100);

	activeSnow_L1 = CreateSnow(aWorld, L1);
	activeSnow_L2 = CreateSnow(aWorld, L2);
	activeSnow_R1 = CreateSnow(aWorld, R1);
	activeSnow_R2 = CreateSnow(aWorld, R2);
	activeSnow_C1 = CreateSnow(aWorld, -100);
	activeSnow_C2 = CreateSnow(aWorld, 100);

	aWorld.registerEntity(deadline);
	aWorld.registerEntity(player_L);
	aWorld.registerEntity(player_R);
	aWorld.registerEntity(heart_L);
	aWorld.registerEntity(heart_R);

	ecs::Entity timer = aWorld.createEntity();
	aWorld.addComponent(timer, Timer());

	aWorld.registerEntity(timer);

	ecs::Entity end = aWorld.createEntity();
	aWorld
		.addComponent(end, Position(0.0f, -632.0f))
		.addComponent(end, Scale(825.0f, 92.0f))
		.addComponent(end, Rotation(0.0f))
		.addComponent(end, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(end, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(end, TexCoords({ 0.0f, 0.0, 1.0f, 0.0f, 1.0f, 1.0, 0.0f, 1.0f }))
		.addComponent(end, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(end, Direction(0.0f, 0.0f))
		.addComponent(end, Speed(500.0f))
		.addComponent(end, BoxCollider(0.0f, 0.0f, 324.0f, 92.0f))
		.addComponent(end, Area(-960.0f, 960.0f, 46.0f, -800.0f))
		.addComponent(end, SpawnInterval(190.0f))
		.addComponent(end, Texture(endTex));
}

E_NoteType ParseNoteType(const std::string& str) {
	if (str == "Tap") return E_NoteType::Tap;
	if (str == "Hold") return E_NoteType::Hold;
	if (str == "DNA") return E_NoteType::DNA;

	throw std::invalid_argument("Unknown NoteType");
}

void LoadNotesFromString(const std::string& data, ecs::World& world) {
	std::istringstream stream(data); // 文字列ストリームを使用してデータを読み込む
	std::string line;
	while (std::getline(stream, line))
	{
		std::replace(line.begin(), line.end(), '\t', ' '); // タブ文字をスペースに置き換える
		std::istringstream iss(line);
		std::string aStr, typeStr;
		float spawnTimeVal;
		float holdDuration = 0.0f; // デフォルト値

		// ノートの基本パラメータを解析する
		if (!(iss >> aStr >> typeStr >> spawnTimeVal >> holdDuration))
		{
			std::cerr << "行の解析に失敗しました: " << line << std::endl;
			continue;
		}

		// 文字列を対応する値に変換する
		float pos_x = 0.0f;
		if (aStr == "L1") pos_x = L1;
		else if (aStr == "L2") pos_x = L2;
		else if (aStr == "R1") pos_x = R1;
		else if (aStr == "R2") pos_x = R2;
		else if (aStr == "DNA") pos_x = C;

		else
		{
			std::cerr << "不明な値です: " << aStr << std::endl;
			continue;
		}

		E_NoteType noteTypeEnum = ParseNoteType(typeStr);

		spawnTimeVal -= 0.89f;

		// ノートタイプに応じて追加パラメータ（HoldStateなど）を読み込む
		if (noteTypeEnum == E_NoteType::Hold)
		{
			noteList.push_back(NoteData(pos_x, noteTypeEnum, spawnTimeVal, holdDuration));
		}
		else if (noteTypeEnum == E_NoteType::Tap)
		{
			noteList.push_back(NoteData(pos_x, noteTypeEnum, spawnTimeVal, 0.0f));
		}
		else if (noteTypeEnum == E_NoteType::DNA)
		{
			noteList.push_back(NoteData(pos_x, noteTypeEnum, spawnTimeVal, holdDuration));
		}
	}
}

int main()
{
	GLFWwindow* window = Renderer::Instance().initWindow(1920, 1080, "OpenGL Square");
	if (!window) return -1;
	ecs::World world;
	CreateComponentStore(world);

	Renderer::Instance().Init();
	InitTexture();

	AddSystem(world);

#pragma region  Note
	std::string noteData = R"(
R1	Tap	2.8	0
R2	Tap	5.54	0
R1	Tap	8.3	0
R2	Tap	10.96	0
L1	Tap	13.36	0
L2	Hold	15.1	2
R1	Tap	18	0
R2	Hold	20.6	2
R1	Tap	23.3	0
R2	Tap	25.3	0
R1	Hold	27.3	1.5
L1	Hold	28.62	1
R2	Hold	30	2
L1	Tap	32.56	0
L2	Hold	33.46	2
L1	Tap	36.72	0
R2	Tap	36.72	0
L2	Tap	37.88	0
R1	Tap	37.88	0
L1	Hold	40.82	2.2
R2	Hold	40.82	2.2
L2	Tap	43.52	0
R1	Tap	43.52	0
R2	Tap	44.74	0
L1	Tap	46.3	0
L2	Tap	47.46	0
R1	Tap	47.46	0
DNA	DNA	49.04	1.6
L1	Hold	50.26	1.22
R2	Hold	50.94	0.86
L2	Hold	51.48	0.86
R1	Hold	51.8	1.22
L1	Tap	53.2	0
R2	Tap	53.68	0
L2	Tap	54.2	0
DNA	DNA	54.58	1.24
L1	Tap	55.82	0
L2	Tap	56.26	0
R1	Tap	56.6	0
R2	Tap	56.96	0
L2	Hold	57.34	1.42
R1	Hold	57.34	2
L1	Hold	58.76	1.86
L2	Hold	60.62	2
R1	Tap	63.52	0
R2	Tap	64.32	0
R1	Tap	64.9	0
R2	Tap	65.68	0
L1	Tap	66.06	0
L2	Hold	66.76	2
DNA	DNA	67.98	2
R2	Hold	69.98	1.12
L1	Hold	70.58	1.46
R1	Hold	71.12	2.92
L2	Hold	72.04	2
DNA	DNA	74.04	2
L1	Tap	75.38	0
R2	Hold	75.72	2.28
L2	Tap	76.5	0
L1	Hold	77.22	2.9
R1	Tap	78	0
R2	Tap	78.66	0
R1	Tap	79.24	0
R2	Tap	79.74	0
L2	Hold	80.12	2.9
R1	Tap	81.24	0
R1	Hold	81.6	1.08
R2	Tap	82.68	0
L1	Hold	83.1	1.32
L2	Tap	84.42	0
L1	Tap	85.14	0
L2	Tap	85.48	0
R1	Tap	85.88	0
R2	Tap	86.22	0
L1	Tap	86.62	0
L2	Tap	86.96	0
R1	Tap	87.32	0
R2	Tap	87.76	0
DNA	DNA	88.26	0.72
R1	Tap	88.98	0
L2	Tap	89.44	0
R2	Tap	89.9	0
L1	Tap	90.42	0
DNA	DNA	91.28	2
R2	Tap	93.34	0
L1	Hold	93.56	1.9
R1	Tap	94.1	0
R2	Tap	94.82	0
L2	Tap	95.48	0
R1	Hold	95.72	0.76
L1	Hold	96.48	2.3
R2	Hold	96.48	2.3
R1	Tap	98.78	0
L2	Hold	99.32	2.36
R2	Tap	101.68	0
R1	Tap	102.2	0
L1	Hold	102.72	2.88
R2	Tap	103.66	0
R1	Tap	104.16	0
R2	Tap	104.7	0
L1	Hold	105.6	0.84
R2	Tap	105.6	0
R1	Hold	105.78	1.08
L2	Tap	106.44	0
L1	Hold	106.86	2.26
R1	Tap	107.8	0
R2	Tap	108.26	0
R1	Tap	109.12	0
R2	Hold	109.84	0.8
L2	Hold	110.64	2.38
R1	Hold	111.78	1.24
L1	Tap	113.02	0
L2	Hold	113.34	2.42
R2	Tap	114.06	0
R1	Hold	114.52	1.24
L1	Tap	115.76	0
R2	Tap	116.04	0
L2	Hold	116.8	1.3
R1	Hold	116.8	1.3
L1	Tap	118.1	0
R2	Tap	118.1	0
DNA	DNA	118.8	2.8
R2	Tap	121	0
L1	Hold	121.48	2.74
R1	Tap	123.88	0
R2	Hold	124.22	1.24
L2	Tap	124.96	0
L1	Hold	125.46	1.5
R1	Tap	126.3	0
R2	Tap	126.96	0
L2	Tap	127.68	0
R1	Tap	127.68	0
L1	Tap	128.22	0
R2	Tap	128.22	0
L2	Tap	129.06	0
R1	Tap	129.06	0
L1	Hold	129.72	2.4
R2	Hold	129.72	1.42
R1	Tap	130.46	0
R2	Tap	131.14	0
L2	Tap	132.12	0
R1	Tap	132.12	0
L1	Hold	132.46	1.56
R2	Hold	132.46	1.56
L2	Tap	134.02	0
R1	Tap	134.02	0
DNA	DNA	135.56	5.08
DNA	DNA	141.16	5.1
L1	Tap	147.1	0
R2	Tap	147.1	0
L2	Tap	147.96	0
R1	Tap	147.96	0
L1	Tap	148.86	0
R2	Tap	148.86	0
L2	Tap	149.9	0
R1	Tap	150.4	0
R2	Tap	150.78	0
L1	Hold	151.1	1.6
R1	Tap	151.48	0
R2	Hold	151.84	0.86
L2	Tap	152.7	0
R1	Tap	152.7	0
L1	Tap	153.24	0
R2	Tap	153.24	0
L2	Tap	153.94	0
R1	Tap	153.94	0
L1	Tap	154.48	0
R2	Tap	154.48	0
L2	Tap	155.2	0
R1	Hold	155.5	1.92
L1	Tap	156.04	0
L2	Tap	156.72	0
L1	Tap	157.42	0
R2	Tap	157.42	0
L2	Hold	157.62	0.7
R1	Hold	157.62	0.7
L1	Tap	158.32	0
R2	Tap	158.64	0
L2	Tap	158.84	0
R1	Tap	159.18	0
R2	Tap	159.54	0
L1	Tap	159.88	0
R1	Tap	160.16	0
L2	Tap	160.5	0
L1	Tap	161.08	0
R2	Tap	161.08	0
L2	Tap	161.4	0
R1	Tap	161.4	0
L1	Tap	161.78	0
R2	Tap	161.78	0
L2	Tap	162.28	0
R1	Tap	162.28	0
L1	Hold	162.8	0.98
R2	Hold	162.8	0.98
L2	Tap	163.78	0
R1	Tap	164.3	0
L1	Tap	164.98	0
R2	Tap	165.34	0
L2	Tap	165.8	0
R1	Tap	166.46	0
L1	Tap	167.06	0
R2	Tap	167.06	0
L2	Tap	167.44	0
R1	Tap	167.44	0
L1	Hold	167.24	1
R2	Hold	167.24	1
R1	Tap	169.24	0
L2	Tap	169.72	0
L1	Tap	170	0
R2	Tap	170	0
L2	Tap	171.14	0
R1	Tap	171.46	0
L1	Hold	172.02	1.18
R2	Hold	172.02	1.18
L2	Tap	173.2	0
R1	Tap	173.54	0
R2	Tap	173.84	0
L1	Tap	174.18	0
L1	Hold	174.56	1.36
R2	Hold	174.56	1.36
L2	Hold	175.92	1.36
R1	Tap	176.4	0
R2	Tap	176.76	0
L1	Hold	177.28	1.44
R1	Tap	177.76	0
R2	Tap	178.24	0
L2	Hold	178.72	1.22
R1	Tap	179.24	0
R2	Tap	179.7	0
L1	Tap	179.94	0
R2	Tap	179.94	0
DNA	DNA	180.48	5
)";
#pragma endregion

	LoadNotesFromString(noteData, world);

	CreateEntity(world);

	auto lastTime = std::chrono::steady_clock::now();

	const int targetFPS = 60;
	const float targetFrameTime = 1.0f / targetFPS;
	float fpsTimeCounter = 0.0f;
	int frameCount = 0;

	AudioManager audioManager;

	if (!audioManager.initialize())
	{
		return -1;
	}

	audioManager.loadSound("My Dearest", "assets/sounds/My Dearest.wav");

	ecs::World title;
	CreateComponentStore(title);
	title
		.addSystem(ecs::System::Ptr(new SystemOrbit(title)))
		.addSystem(ecs::System::Ptr(new SystemRenderShape(title)));
	ecs::Entity cover = title.createEntity();
	title
		.addComponent(cover, Position(0.0f, 0.0f))
		.addComponent(cover, Scale(1122.0f, 1123.0f, 0.8))
		.addComponent(cover, Rotation(0.0f))
		.addComponent(cover, Color(1.0f, 1.0f, 1.0f, 1.0f))
		.addComponent(cover, Vertex({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,0.5f, 0.5f, 0.0f,-0.5f, 0.5f, 0.0f }))
		.addComponent(cover, TexCoords({ 0.0f, 0.0, 1.0f, 0.0f, 1.0f, 1.0, 0.0f, 1.0f }))
		.addComponent(cover, Indices({ 0, 1, 2, 0, 2, 3 }))
		.addComponent(cover, Orbit(20.0f, true, true, 0.0f, 0.0f))
		.addComponent(cover, Texture(coverTex));
	title.registerEntity(cover);

	bool isBgmStart = false;

	while (!glfwWindowShouldClose(window))
	{
		auto currentTime = std::chrono::steady_clock::now();

		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 更新所有实体
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			audioManager.playBGM("assets/sounds/My Dearest.wav");
			isBgmStart = true;
		}

		if (isBgmStart)
		{
			world.updateEntities(deltaTime);
		}
		else
		{

			title.updateEntities(deltaTime);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		float frameTime = deltaTime;
		if (frameTime < targetFrameTime)
			std::this_thread::sleep_for(std::chrono::duration<float>(targetFrameTime - frameTime));

		fpsTimeCounter += deltaTime;
		frameCount++;

		if (fpsTimeCounter >= 1.0f) 
		{ 
			float fps = frameCount / fpsTimeCounter;
			fpsTimeCounter -= 1.0f;
			frameCount = 0;
		}
	}

	Renderer::Instance().CleanUp();

	audioManager.cleanup();

	glfwTerminate();
	return 0;
}