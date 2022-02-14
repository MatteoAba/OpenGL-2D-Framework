#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../cglph.h"

class Application;

struct WindowProperties 
{
	int width;
	int height;
	std::string title;

	WindowProperties(int w, int h, std::string t) 
		: width(w), height(h), title(t) {}
};

class Window 
{
public:
	Window(Application* owner, WindowProperties properties);
	~Window();
	void ProcessEventBuffer();

	inline GLFWwindow* GetWindow() { return m_Window; }
	inline Application* GetOwner() { return m_Owner; }
	inline float GetFrameTime() { return (float)glfwGetTime(); }
	inline void SetVSync(int value) { glfwSwapInterval(value); }
	inline GLFWwindow* GetNativeWindow() { return m_Window; }

public:
	// callback eventi (fanno il dispatch dell'evento all'applicazione)
	void WindowResize(int width, int height);
	void KeybordButton(int key, int action);
	void MouseButton(int button, int action);
	void MouseMovement(double xpos, double ypos);
	void MouseScroll(double xoffset, double yoffset);

private:
	WindowProperties m_Properties;
	GLFWwindow* m_Window;
	Application* m_Owner;
};