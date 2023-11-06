/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:10:20 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/06 20:01:12 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

std::vector<Server> ConfigParser::servers;

ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
}

void ConfigParser::parseConfigFile(std::string const &configPath)
{
    std::ifstream file(configPath.c_str());
    std::string line;
    std::stack<state> stateStack;
    stateStack.push(NONE);
    int lineNb = 0;

    if (!file.is_open())
        throw ServerException("Cannot open config file", lineNb);
    while (std::getline(file, line))
    {
        lineNb++;
        trim(line);
        if (line.empty())
            continue;
        else if (line == "<server>") {
            if (stateStack.top() != NONE)
                throw ServerException("Server block not closed", lineNb);
            stateStack.push(SERVER);
            Server server = parseServer(file, line, lineNb, stateStack);
            servers.push_back(server);
            continue;
        }
        else if (line == "</server>") {
            throw ServerException("Server block not opened", lineNb);
        }
        else if (line.find("location") != std::string::npos) {
            throw ServerException("Location block is invalid", lineNb);
        }
        else {
            throw ServerException("Invalid config file", lineNb);
        }
    }
}


Server ConfigParser::parseServer(std::ifstream & file, std::string & line, int & lineNb, std::stack<state> & stateStack)
{
    std::cout << "Server block opened" << std::endl;
    stateStack.push(SERVER);
    Server server;
    while (std::getline(file, line)) 
    {
        lineNb++;
        trim(line);
        if (line.empty())
            continue;
        else if (line == "</server>") {
            if (stateStack.top() != SERVER)
                throw ServerException("Server block not opened", lineNb);
            stateStack.pop();
            std::cout << "Server block closed" << std::endl;
            break;
        }
        else if (line.substr(0, 9) == "<location") {
            std::string path = findLocation(line, lineNb);
            if (!path.empty()) {
                Location location = parseLocation(file, line, lineNb, stateStack);
                location.setPath(path);
                server.addLocation(location);
            }
            continue;
        }
        else if (line == "</location>") {
            throw ServerException("Location block not opened", lineNb);
        }
        else if (line.find("=") == std::string::npos)
            throw ServerException("Invalid server block", lineNb);
        server.fill(line, lineNb);
    }
    return server;
}

Location ConfigParser::parseLocation(std::ifstream & file, std::string & line, int & lineNb, std::stack<state> & stateStack)
{
    std::cout << "Location block opened" << std::endl;
    stateStack.push(LOCATION);
    Location location;
   while (std::getline(file, line)) 
    {
        lineNb++;
        trim(line);
        if (line.empty())
            continue;
        else if (line == "</location>") {
            if (stateStack.top() != LOCATION)
                throw ServerException("Location block not opened", lineNb);
            stateStack.pop();
            std::cout << "Location block closed" << std::endl;
            break; 
        }
        else if (line.substr(0, 9) == "<location") {
            std::string path = findLocation(line, lineNb);
            if (!path.empty()) {
                Location location = parseLocation(file, line, lineNb, stateStack);
                location.setPath(path);
                location.addLocation(location);
            }
            continue;
        }
        else if (line.find("=") == std::string::npos)
            throw ServerException("Invalid location block", lineNb);
        // std::cout << line << std::endl;
    } 
    return location;
}

std::string ConfigParser::findLocation(std::string line, int lineNb)
{
    std::string path;
    size_t pathPos = line.find("path=\"");
    if (pathPos == std::string::npos) {
        throw ServerException("Invalid location block, path not found", lineNb);
    }
    size_t endPos = line.find("\"", pathPos + 6);
    if (endPos == std::string::npos) {
        throw ServerException("Invalid location block, unclosed path", lineNb);
    }
    path = line.substr(pathPos + 6, endPos - pathPos - 6);
    line.erase(pathPos, endPos - pathPos + 1);
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    
    if (line != "<location>") {
        throw ServerException("Invalid location block", lineNb);
    }
    return path;
}

std::vector<Server> ConfigParser::getServers()
{
    return servers;
}