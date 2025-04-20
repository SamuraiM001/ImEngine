#include "Runtime.h"

void Runtime::Initialize(int argc, char* argv[]){
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 600, "Runtime View");

}

void Runtime::Run(){
    BeginDrawing();


    EndDrawing();

}

void Runtime::Shutdown()
{
}
