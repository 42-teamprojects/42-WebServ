#pragma once

#include "webserv.hpp"

class ServerContext
{
private:
    std::string                 listen;
    std::string                 host;
    std::vector<std::string>    serverName;
    std::vector<std::string>    errorPage;
    size_t                      clientMaxBodySize;
    std::string                 root;
    std::vector<std::string>    index;
    bool                        autoIndex;
    std::vector<Location>       location;
public:
    ServerContext(/* args */);
    ~ServerContext();

    // Getters
    std::vector<std::string>    getListen() const;
    std::vector<std::string>    getHost() const;
    std::vector<std::string>    getServerContextName() const;
    std::vector<std::string>    getErrorPage() const;
    int                         getClientMaxBodySize() const;
    std::string                 getRoot() const;
    std::vector<std::string>    getIndex() const;
    bool                        getAutoIndex() const;
    std::vector<Location>       getLocation() const;

    // Setters
    void                        setListen(std::string listen);
    void                        setHost(std::string host);
    void                        setServerContextName(std::vector<std::string> serverName);
    void                        setErrorPage(std::vector<std::string> errorPage);
    void                        setClientMaxBodySize(int clientMaxBodySize);
    void                        setRoot(std::string root);
    void                        setIndex(std::vector<std::string> index);
    void                        setAutoIndex(bool autoIndex);
    void                        setLocation(std::vector<Location> location);
};
    