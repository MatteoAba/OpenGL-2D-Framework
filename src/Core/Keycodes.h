#pragma once

enum class Keycode
{
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_ESC,
	MOUSE_1,
	MOUSE_2,
	KEYCODE_SIZE
};

enum class KeyEventType
{
    PRESSED,
    REPEATED,
    RELEASED
};

enum class MouseEventType
{
    CLICK,
    UNCLICK
};