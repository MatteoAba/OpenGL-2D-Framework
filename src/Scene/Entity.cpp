#include "Entity.h"

Entity::~Entity()
{
	// remove all the components
	for (auto component : m_Components)
		RemoveComponent(component);
}

void Entity::OnEvent(Event e)
{
	if (m_State == EntityState::ACTIVE) {
		// dispatch events to all the components
		for (auto component : m_Components)
			component->OnEvent(e);

		// dispatch events to the entity
		OnEventEntity(e);
	}
}

void Entity::OnUpdate(float ts)
{
	if (m_State == EntityState::ACTIVE) {
		// update all the components
		for (auto component : m_Components)
			component->OnUpdate(ts);

		// update the entity
		OnUpdateEntity(ts);
	}
}

void Entity::AddComponent(Component* component)
{
	uint32_t insertOrder = component->GetUpdateOrder();
	
	// search first component with higher insert order
	auto iter = m_Components.begin();
	for (;iter != m_Components.end(); ++iter)
		if (insertOrder < (*iter)->GetUpdateOrder())
			break;

	// inserts element before it
	m_Components.insert(iter, component);
}

void Entity::RemoveComponent(Component* component)
{
	auto iter = std::find(m_Components.begin(), m_Components.end(), component);
	if (iter != m_Components.end())
		m_Components.erase(iter);
}
