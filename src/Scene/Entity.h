#pragma once

#include "../Core/Application.h"
#include "Component.h"

enum class EntityState
{
	ACTIVE,
	PAUSED,
	DEAD
};

class Entity
{
public:
	Entity(Application* owner)
		: m_Owner(owner), m_State(EntityState::ACTIVE), m_Position(glm::vec3(0.0f)), m_Rotation(0.0f), m_Scale(1.0f) {}
	~Entity();

	// game loop utilities
	void OnEvent(Event e);											// call OnEvent for all components and the OnEventEntity()
	virtual void OnEventEntity(Event e) {};							// implementation based
	void OnUpdate(float ts);										// call OnUpdate for all components and the OnUpdateEntity()
	virtual void OnUpdateEntity(float ts) {};						// implementation based

	// component utilities
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	// getters and setters
	inline EntityState GetState() { return m_State; }
	inline const glm::vec3& GetPosition() { return m_Position; }
	inline void SetPosition(const glm::vec3& position) { m_Position = position; }
	inline float GetRotation() { return m_Rotation; }
	inline void SetRotation(float rotation) { m_Rotation = rotation; }
	inline float GetScale() { return m_Scale; }
	inline void SetScale(float scale) { m_Scale = scale; }

private:
	Application* m_Owner;
	std::vector<Component*> m_Components;
	EntityState m_State;

	// scene options
	glm::vec3 m_Position;
	float m_Rotation;
	float m_Scale;
};