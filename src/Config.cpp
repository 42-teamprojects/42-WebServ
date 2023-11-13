/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:10:20 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/12 18:31:15 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::vector<Server> Config::servers;

Config::Config()
{
}

Config::~Config()
{
}

void Config::parse(std::string const &configPath)
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
        else if (line == "<server>")
        {
            if (stateStack.top() != NONE)
                throw ServerException("Server block not closed", lineNb);
            Server server = parseServer(file, line, lineNb, stateStack);
            servers.push_back(server);
            continue;
        }
        else if (line == "</server>")
            throw ServerException("Server block not opened", lineNb);
        else if (line.find("route") != std::string::npos)
            throw ServerException("Route block is invalid", lineNb);
        else
            throw ServerException("Invalid config file", lineNb);
    }
}

Server Config::parseServer(std::ifstream &file, std::string &line, int &lineNb, std::stack<state> &stateStack)
{
    stateStack.push(SERVER);
    Server server = Server();
    while (std::getline(file, line))
    {
        lineNb++;
        trim(line);
        if (line.empty())
            continue;
        else if (line == "</server>")
        {
            if (stateStack.top() != SERVER)
                throw ServerException("Server block not opened", lineNb);
            stateStack.pop();
            break;
        }
        else if (line.substr(0, 6) == "<route")
        {
            std::string path = findRoute(line, lineNb);
            if (!path.empty())
            {
                Route route = parseRoute(file, line, lineNb, stateStack);
                route.setPath(path);
                server.addRoute(route);
            }
            continue;
        }
        else if (line == "</route>")
            throw ServerException("Route block not opened", lineNb);
        else if (line.find("=") == std::string::npos)
            throw ServerException("Invalid server block", lineNb);
        // else
        //     throw ServerException("Invalid server block", lineNb);

        server.fill(line, lineNb);
    }
    return server;
}

Route Config::parseRoute(std::ifstream &file, std::string &line, int &lineNb, std::stack<state> &stateStack)
{
    stateStack.push(ROUTE);
    Route route = Route();
    while (std::getline(file, line))
    {
        lineNb++;
        trim(line);
        if (line.empty())
            continue;
        else if (line == "</route>")
        {
            if (stateStack.top() != ROUTE)
                throw ServerException("Route block not opened", lineNb);
            stateStack.pop();
            break;
        }
        else if (line.substr(0, 6) == "<route")
            throw ServerException("Route block not closed", lineNb);
        else if (line.find("=") == std::string::npos)
            throw ServerException("Invalid route block", lineNb);
        route.fill(line, lineNb);
    }
    return route;
}

std::string Config::findRoute(std::string line, int lineNb)
{
    std::string path;
    size_t pathPos = line.find("path=\"");
    if (pathPos == std::string::npos)
    {
        throw ServerException("Invalid route block, path not found", lineNb);
    }
    size_t endPos = line.find("\"", pathPos + 6);
    if (endPos == std::string::npos)
    {
        throw ServerException("Invalid route block, unclosed path", lineNb);
    }
    path = line.substr(pathPos + 6, endPos - pathPos - 6);
    line.erase(pathPos, endPos - pathPos + 1);
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

    if (line != "<route>")
    {
        throw ServerException("Invalid route block", lineNb);
    }
    return path;
}

std::vector<Server> Config::getServers()
{
    return servers;
}