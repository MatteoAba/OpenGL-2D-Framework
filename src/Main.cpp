#include "Core/Application.h"

int main(void)
{
    Application app(1280, 720, "ChessGL");
    app.Inizialize();
    app.Run();
    
    return 0;
}