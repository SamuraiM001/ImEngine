#include "ImEngine.h"
#include "Editor.h"

int main(int argc, char* argv[]){

    Editor Ed = Editor();

    Ed.Initialize(argc,  argv);
    Ed.Run();
    Ed.Shutdown();


    return 0;
}
