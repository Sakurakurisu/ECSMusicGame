#pragma once

#include "System.h"

class SystemInput : public ecs::System
{
public:
	SystemInput(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Input::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		Input& input = mWorld.getComponentStore<Input>().get(aEntity);

		// 各キーの現在の押下状態をGLFWから取得
		input.keyStates[E_KeyCode::keyCodeDown_W] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_W) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_A] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_A) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_S] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_S) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_D] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_D) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_Up] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_UP) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_LEFT] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_Down] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_RIGHT] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_E] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_E) == GLFW_PRESS;
		input.keyStates[E_KeyCode::keyCodeDown_R] = glfwGetKey(Renderer::Instance().GetWindow(), GLFW_KEY_R) == GLFW_PRESS;
	}
};
