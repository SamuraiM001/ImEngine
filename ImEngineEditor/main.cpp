#include "ImEngine.h"
#include "Editor.h"

int main(int argc, char* argv[]){
    if (argc > 1) {
        Editor Ed = Editor();
        Ed.Initialize(argc,  argv);
        Ed.Run();
        Ed.Shutdown();
    }
    return 0;
}


