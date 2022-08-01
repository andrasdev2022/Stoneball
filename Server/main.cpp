#include "mock.h"
#include "ServerApplication.h"

#include <memory>
#include <gtest/gtest.h>
#include "ParameterParser.h"

int main(int argc, char* argv[]) {
    ParameterParser pparser(argc, (const char**)argv);
    int port = 44444;
    size_t portPos = pparser.find("-port");
    if(portPos + 1 < pparser.size()) {
        port = std::atoi(pparser.at(portPos + 1).c_str());
    }

    int mapIndex = 0;
    size_t mapPos = pparser.find("-map");
    if(mapPos + 1 < pparser.size()) {
        mapIndex = std::atoi(pparser.at(mapPos + 1).c_str());
    }
    int minutes = 5;
    size_t minutesPos = pparser.find("-minutes");
    if(minutesPos + 1 < pparser.size()) {
        minutes = std::atoi(pparser.at(minutesPos + 1).c_str());
    }

    ServerApplication::InitServer(port, mapIndex, minutes);
    ServerApplication::RunServer();
    return EXIT_SUCCESS;
}
