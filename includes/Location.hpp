#pragma once

#include "webserv.hpp"

class Location
{
private:
    std::vector<std::string>    errorPage;
    std::string                 root;
    std::vector<std::string>    index;
    bool                        autoIndex;
    std::string                 redir;
    std::vector<std::string>    allowMethods;
    size_t                      clientMaxBodySize;
    // CgiInfo                     cgi;
    std::vector<Location>       location;
public:
    Location(/* args */);
    ~Location();

    // Getters
    std::vector<std::string>    getErrorPage() const;
    std::string                 getRoot() const;
    std::vector<std::string>    getIndex() const;
    bool                        getAutoIndex() const;
    std::string                 getRedir() const;
    std::vector<std::string>    getAllowMethods() const;
    size_t                      getClientMaxBodySize() const;
    // CgiInfo                     getCgi() const;
    std::vector<Location>       getLocation() const;

    // Setters
    void                        setErrorPage(const std::vector<std::string>& errorPage);
    void                        setRoot(const std::string& root);
    void                        setIndex(const std::vector<std::string>& index);
    void                        setAutoIndex(bool autoIndex);
    void                        setRedir(const std::string& redir);
    void                        setAllowMethods(const std::vector<std::string>& allowMethods);
    void                        setClientMaxBodySize(size_t clientMaxBodySize);
    // void                        setCgi(const CgiInfo& cgi);
    void                        setLocation(const std::vector<Location>& location);
};
