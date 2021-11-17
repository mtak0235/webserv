#include <iostream>
#include <string>
#include "Config.hpp"

int main() {
    
    typedef std::vector<Config::ServerConfig>        ServerConfig;

    Config c;
    c.parse();

    ServerConfig sc = c.getServerConfig();

    for (size_t i = 0; i < sc.size(); i++) {
        std::cout << sc[i].serverName << " [PORT NUM : ";
        std::cout << sc[i].serverPort << "] \n";
    }

    return 0;
}