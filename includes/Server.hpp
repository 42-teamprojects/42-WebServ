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
#include "Route.hpp"

class Server
{
private:
    int                             port;
    std::string                     host;
    std::vector<std::string>        serverNames;
    size_t                          clientMaxBodySize;
    std::string                     root;
    std::vector<std::string>        index;
    std::vector<std::string>        errorPages;
    bool                            allowListing;
    std::vector<Route>              routes;

public:
    Server();
    ~Server();

    // Getters
    int                         getPort() const;
    std::string                 getHost() const;
    std::vector<std::string>    getServerNames() const;
    size_t                      getClientMaxBodySize() const;
    std::string                 getRoot() const;
    std::vector<std::string>    getIndex() const;
    std::vector<std::string>    getErrorPages() const;
    bool                        getAllowListing() const;
    std::vector<Route>          getRoutes() const;

    // Setters
    void                        setPort(const int &);
    void                        setHost(const std::string &);
    void                        setServerNames(const std::vector<std::string>&);
    void                        setClientMaxBodySize(const size_t &);
    void                        setRoot(const std::string &);
    void                        setAllowListing(const bool &);
    void                        setIndex(const std::vector<std::string>&);
    void                        setErrorPages(const std::vector<std::string>&);
    void                        addRoute(Route route);
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
            if (it->getPath() == path) {
                return it;
            }
        }
        return routes.end();
    }
};
    