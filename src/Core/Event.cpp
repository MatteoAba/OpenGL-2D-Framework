#include "Event.h"
#include "Log.h"

Event::Event(Keycode button, KeyEventType action)
	: m_Type(EventType::KEYBTN)
{
	m_Structure.Key = { button, action };
}

Event::Event(Keycode button, MouseEventType action)
	: m_Type(EventType::MOUSEBTN)
{
	m_Structure.Mouse = { button, action };
}

Event::Event(uint32_t x, uint32_t y)
	: m_Type(EventType::MOUSEPOS)
{
	m_Structure.Pos = { x, y };
}

Event::Event(uint16_t xOffset, uint16_t yOffset)
	: m_Type(EventType::MOUSESCROLL)
{
	m_Structure.Scroll = { xOffset, yOffset };
}

Event::Event(uint32_t width, uint32_t height, uint8_t id)
	: m_Type(EventType::VIEWPORTRESIZE)
{
	m_Structure.Resize = { width, height, id };
}

void Event::Print()
{
	switch (m_Type)
	{
	case EventType::KEYBTN:
		LOG_INFO("KEY BUTTON EVENT: {} {}", m_Structure.Key.button, m_Structure.Key.action);
		break;
	case EventType::MOUSEBTN:
		LOG_INFO("MOUSE BUTTON EVENT: {} {}", m_Structure.Mouse.button, m_Structure.Mouse.action);
		break;
	case EventType::MOUSEPOS:
		LOG_INFO("MOUSE POSITION: {} {}", m_Structure.Pos.x, m_Structure.Pos.y);
		break;
	case EventType::MOUSESCROLL:
		LOG_INFO("MOUSE SCROLL: {} {}", m_Structure.Scroll.xOffset, m_Structure.Scroll.yOffset);
		break;
	case EventType::VIEWPORTRESIZE:
		LOG_INFO("VIEWPORT RESIZE: {} {} {}", m_Structure.Resize.width, m_Structure.Resize.height, m_Structure.Resize.viewportId);
		break;
	default:
		LOG_WARN("EVENT TYPE UNKNOWN");
		break;
	};
}
