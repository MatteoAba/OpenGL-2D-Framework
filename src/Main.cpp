#include "Core/Application.h"

int main(void)
{
    Application app(640, 480, "Test");
    app.Inizialize();
    app.Run();
    
    return 0;
}