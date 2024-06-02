#include "Core/Application.h"

int main(void)
{
    Application app(1280, 720, "OpenGL 2D Framework");
    app.Inizialize();
    app.Run();
    
    return 0;
}