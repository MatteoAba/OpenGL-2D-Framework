#pragma once

#include "../cglph.h"
#include "Keycodes.h"

enum class EventType
{
    KEYBTN,
    MOUSEBTN,
    MOUSEPOS,
    MOUSESCROLL
};

union EventStructure
{
    struct KeyBtn
    {
        Keycode button;
        KeyEventType action;
    } Key;

    struct MouseBtn
    {
        Keycode button;
        MouseEventType action;
    } Mouse;

    struct MousePos
    {
        uint32_t x;
        uint32_t y;
    } Pos;

    struct MouseScroll
    {
        uint16_t xOffset;
        uint16_t yOffset;
    } Scroll;
};

class Event
{
public:
   Event(Keycode button, KeyEventType action);
   Event(Keycode button, MouseEventType action);
   Event(uint32_t x, uint32_t y);
   Event(uint16_t xOffset, uint16_t yOffset);

   void Print();
   inline EventType GetType() { return m_Type; }
   inline EventStructure GetStructure() { return m_Structure; }

private:
    EventType m_Type;
    EventStructure m_Structure;
};