#include "Editor.h"
#include "ProjectManager.h"
#include <string>

int main(int argc, char* argv[]) {
    std::string choice;

    if (argc < 2) {
        ProjectManager manager;
        choice = manager.GetProjectPath();
    }
    else {
        choice = argv[1];
    }

    if (choice.empty()) return 1;
    // Simulate passing path as argv
    const char* fakeArgv[] = { "ImEngineEditor.exe", choice.c_str() };
    int fakeArgc = 2;

    Editor editor;
    editor.Initialize(fakeArgc, const_cast<char**>(fakeArgv));
    editor.Run();
    editor.Shutdown();

    return 0;
}
