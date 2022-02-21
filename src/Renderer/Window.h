#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../cglph.h"

class Application;

struct WindowProperties 
{
	uint32_t width;
	uint32_t height;
	std::string title;

	WindowProperties(uint32_t w, uint32_t h, std::string t)
		: width(w), height(h), title(t) {}
};

struct ViewportProperties
{
	uint32_t width;
	uint32_t height;

	ViewportProperties(uint32_t w, uint32_t h)
		: width(w), height(h){}
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
	inline void SetVSync(uint32_t value) { glfwSwapInterval(value); }
	inline GLFWwindow* GetNativeWindow() { return m_Window; }

	// setters and getters for windows properties
	inline void SetWidth(uint32_t width)    { m_Properties.width = width; }
	inline void SetHeight(uint32_t height)  { m_Properties.height = height; }
	inline void SetTitle(std::string title) { m_Properties.title = title;  glfwSetWindowTitle(m_Window, title.c_str()); }
	inline uint32_t  GetWidth()   { return m_Properties.width; }
	inline uint32_t  GetHeight()  { return m_Properties.height; }
	inline std::string GetTitle() { return m_Properties.title; }

	// setters and getters for viewport properties
	inline void SetViewportWidth(uint32_t width)   { m_ViewportProperties.width = width; }
	inline void SetViewportHeight(uint32_t height) { m_ViewportProperties.height = height; }
	inline void SetIsViewport(bool value) { m_IsViewport = value; }
	inline uint32_t  GetViewportWidth()  { return m_ViewportProperties.width; }
	inline uint32_t  GetViewportHeight() { return m_ViewportProperties.height; }
	inline bool GetIsViewport() { return m_IsViewport; }
	void UpdateViewport(uint32_t width, uint32_t height);

public:
	// events callback (dispatch events to application)
	void WindowResize(uint32_t width, uint32_t height);
	void ViewportResize(uint32_t width, uint32_t height);
	void KeybordButton(uint32_t key, uint32_t action);
	void MouseButton(uint32_t button, uint32_t action);
	void MouseMovement(double xpos, double ypos);
	void MouseScroll(double xoffset, double yoffset);

private:
	WindowProperties m_Properties;
	ViewportProperties m_ViewportProperties;
	bool m_IsViewport;
	GLFWwindow* m_Window;
	Application* m_Owner;
};