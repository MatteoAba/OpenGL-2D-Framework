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

	// setters and getters for members
	inline GLFWwindow* GetWindow() { return m_Window; }
	inline Application* GetOwner() { return m_Owner; }
	inline float GetFrameTime() { return (float)glfwGetTime(); }
	inline void SetVSync(int value) { glfwSwapInterval(value); }
	inline GLFWwindow* GetNativeWindow() { return m_Window; }

	// setters and getters for windows properties
	inline void SetWidth(int width) { m_Properties.width = width; }
	inline void SetHeight(int height) { m_Properties.height = height; }
	inline void SetTitle(std::string title) { m_Properties.title = title;  glfwSetWindowTitle(m_Window, title.c_str()); }
	inline int GetWidth() { return m_Properties.width; }
	inline int GetHeight() { return m_Properties.height; }
	inline std::string GetTitle() { return m_Properties.title; }

public:
	// events callback (dispatch events to application)
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