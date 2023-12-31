/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:51:41 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 14:15:27 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "Route.hpp"

void    trimTrailingSlashes(std::string & s);

class Server
{
private:
    int                                         port;
    std::string                                 host;
    std::vector<std::string>                    serverNames;
    size_t                                      clientMaxBodySize;
    std::string                                 root;
    std::vector<std::string>                    index;
    std::map<int, std::string>       errorPages;
    bool                                        allowListing;
    std::vector<Route>                          routes;

public:
    Server();
    ~Server();

    // Getters
    int                                         getPort() const;
    std::string                                 getHost() const;
    std::vector<std::string>                    getServerNames() const;
    size_t                                      getClientMaxBodySize() const;
    std::string                                 getRoot() const;
    std::vector<std::string>                    getIndex() const;
    std::map<int, std::string>       getErrorPages() const;
    bool                                        getAllowListing() const;
    std::vector<Route>                          getRoutes() const;

    // Setters
    void                                        setPort(const int &);
    void                                        setHost(const std::string &);
    void                                        setServerNames(const std::vector<std::string>&);
    void                                        setClientMaxBodySize(const size_t &);
    void                                        setRoot(const std::string &);
    void                                        setAllowListing(const bool &);
    void                                        setIndex(const std::vector<std::string>&);
    void                                        setErrorPages(const std::map<int, std::string>&);
    void                                        addRoute(Route route);
    // Methods
    void                        print() const;
    void                        fill(std::string const &, int &);

    // Helpers
    typedef std::vector<Route>::iterator iterator;

    iterator begin() { return routes.begin(); }
    iterator end() { return routes.end(); }

    iterator find(const std::string& path)
    {
        for (iterator it = routes.begin(); it != routes.end(); ++it) {
            // if (it->getPath() == path || isPathMatched(it->getPath(), path)) {
            std::string tmp = path;
            trimTrailingSlashes(tmp);
            if (it->getPath() == tmp) {
                return it;
            }
        }
        return routes.end();
    }
};
    