#include "Core.h"

using namespace IE;

std::string IE::Core::m_WorkFolder = "";

void Core::Initialize(int argc, char* argv[]) {
    if (argc > 1) {
        m_WorkFolder = argv[1]; // 👈 Set the path from command-line argument
        IE_LOG("Work folder set to: " + m_WorkFolder);
    }
    else {
        IE_LOG_ERROR("No Project Path is found");
    }
}



void Core::Shutdown()
{

}
