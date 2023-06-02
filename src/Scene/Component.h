#pragma once

#include "../cglph.h"
#include "../Core/Event.h"

class Entity;

class Component
{
public:
	Component(Entity* owner, uint32_t updateOrder = 100)
		: m_Owner(owner), m_UpdateOrder(updateOrder) {};
	~Component() {};

	inline uint32_t GetUpdateOrder() { return m_UpdateOrder; }
	inline Entity* GetOwner() { return m_Owner; }

	virtual void OnEvent(Event e) {}
	virtual void OnUpdate(float ts) {}

private:
	Entity* m_Owner;
	uint32_t m_UpdateOrder;
};