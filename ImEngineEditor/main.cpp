#include "ImEngine.h"
#include "Editor.h"

int main(int argc, char* argv[]){

    Editor Ed = Editor();

    Ed.Initialize();
    Ed.Run();
    Ed.Shutdown();


    return 0;
}
