#include "includes/webserv.hpp"

std::string isPathMatched(std::string serverRootPath, std::string path) {
    // Remove trailing slashes from server root path and path
    while (!serverRootPath.empty() && serverRootPath.back() == '/')
        serverRootPath.pop_back();
    while (!path.empty() && path.back() == '/')
        path.pop_back();

    // Check if the server root path is a prefix of the path
    if (path.find(serverRootPath) == 0) {
        // Check if the next character after the server root path is a slash or the end of the string
        if (path.length() == serverRootPath.length() || path[serverRootPath.length()] == '/')
            return path.substr(serverRootPath.length() + 1); // Return the new path excluding the root path
    }
    return path; // Return the original path if it doesn't match the server root path
}

int main() {
    std::string serverRootPath = "/tmp/www";
    std::string path = "/tmp/www/asdf";

    std::string newPath = isPathMatched(serverRootPath, path);

    std::cout << "New path: " << newPath << std::endl;

    return 0;
}