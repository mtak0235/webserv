#include <iostream>
#include <string>
#include "ConfigParser.hpp"

int main() {
    
    typedef std::vector<ConfigParser::Server>        ServerConfig;

    ConfigParser cp;
    cp.parse();

    ServerConfig sc = cp.getServerConfig();

    for (size_t i = 0; i < sc.size(); i++) {
        std::cout << sc[i].serverName << " [PORT NUM : ";
        std::cout << sc[i].serverPort << "] \n";
    }

    return 0;
}