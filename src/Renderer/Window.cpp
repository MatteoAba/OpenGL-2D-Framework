#include "../Core/Application.h"
#include "../Core/Log.h"

#include <stb_image.h>

Window::Window(Application* owner, WindowProperties properties)
    : m_Owner(owner), m_Properties(properties.width, properties.height, properties.title)
{
    // OpenGL initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window and context creation
    m_Window = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), NULL, NULL);
    if (m_Window == NULL) {
        LOG_CRITICAL("Creazione dell'oggetto Window fallita!");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_Window);
    
    // set pointer to window, so it is reachable from the lamda function in glfw callback 
    glfwSetWindowUserPointer(m_Window, this);

    // events dispath
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        //static_cast<Window*>(glfwGetWindowUserPointer(window))->WindowResize(width, height);
    });
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->KeybordButton(key, action);
    });
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->MouseButton(button, action);
    });
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->MouseMovement(xpos, ypos);
    });
    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->MouseScroll(xoffset, yoffset);
    });

    // GLAD for OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_CRITICAL("Inizializzazione di GLAD fallita");
        return;
    }

    // system log
    LOG_INFO("Contesto OpenGL inizializzato correttamente");
    LOG_INFO("Vendor: {}", glGetString(GL_VENDOR));
    LOG_INFO("Driver: {}", glGetString(GL_RENDERER));
    LOG_INFO("Version OpenGL: {}", glGetString(GL_VERSION));

    // set the icon
    GLFWimage images[1];
    images[0].pixels = stbi_load("assets/Img/icon.png", &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(m_Window, 1, images);
    stbi_image_free(images[0].pixels);

    // anti aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    LOG_INFO("Anti-aliasing MSAA x4 attivato");
}

Window::~Window()
{
    LOG_TRACE("Eliminazione finestra");
    glfwTerminate();
    LOG_INFO("Finestra eliminata correttamente");
}

void Window::ProcessEventBuffer()
{
    // swap buffer and I/O events handling by glfw
    glfwSwapBuffers(m_Window);
    glfwPollEvents();

    // check if the window should be closed
    if (glfwWindowShouldClose(m_Window))
        m_Owner->SetRunning(false);
}

void Window::WindowResize(int width, int height)
{
    // update viewport
    glViewport(0, 0, width, height);

    // update window properties
    SetWidth(width);
    SetHeight(height);
}

void Window::KeybordButton(int key, int action)
{
    // key W
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        m_Owner->PushEvent(Event(Keycode::KEY_W, KeyEventType::PRESSED));
    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
        m_Owner->PushEvent(Event(Keycode::KEY_W, KeyEventType::REPEATED));
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        m_Owner->PushEvent(Event(Keycode::KEY_W, KeyEventType::RELEASED));
    
    // key A
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        m_Owner->PushEvent(Event(Keycode::KEY_A, KeyEventType::PRESSED));
    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
        m_Owner->PushEvent(Event(Keycode::KEY_A, KeyEventType::REPEATED));
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        m_Owner->PushEvent(Event(Keycode::KEY_A, KeyEventType::RELEASED));

    // key S
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        m_Owner->PushEvent(Event(Keycode::KEY_S, KeyEventType::PRESSED));
    if (key == GLFW_KEY_S && action == GLFW_REPEAT)
        m_Owner->PushEvent(Event(Keycode::KEY_S, KeyEventType::REPEATED));
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        m_Owner->PushEvent(Event(Keycode::KEY_S, KeyEventType::RELEASED));

    // key D
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        m_Owner->PushEvent(Event(Keycode::KEY_D, KeyEventType::PRESSED));
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
        m_Owner->PushEvent(Event(Keycode::KEY_D, KeyEventType::REPEATED));
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        m_Owner->PushEvent(Event(Keycode::KEY_D, KeyEventType::RELEASED));

    // key ESC
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        m_Owner->PushEvent(Event(Keycode::KEY_ESC, KeyEventType::PRESSED));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_REPEAT)
        m_Owner->PushEvent(Event(Keycode::KEY_ESC, KeyEventType::REPEATED));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        m_Owner->PushEvent(Event(Keycode::KEY_ESC, KeyEventType::RELEASED));
}

void Window::MouseButton(int button, int action)
{
    // mouse 1
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
        m_Owner->PushEvent(Event(Keycode::MOUSE_1, MouseEventType::CLICK));
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
        m_Owner->PushEvent(Event(Keycode::MOUSE_1, MouseEventType::UNCLICK));

    // mouse 2
    if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
        m_Owner->PushEvent(Event(Keycode::MOUSE_2, MouseEventType::CLICK));
    if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
        m_Owner->PushEvent(Event(Keycode::MOUSE_2, MouseEventType::UNCLICK));
}

void Window::MouseMovement(double xpos, double ypos)
{
    m_Owner->PushEvent(Event((uint32_t)xpos, (uint32_t)ypos));
}

void Window::MouseScroll(double xoffset, double yoffset)
{
    m_Owner->PushEvent(Event((uint16_t)xoffset, (uint16_t)yoffset));
}
