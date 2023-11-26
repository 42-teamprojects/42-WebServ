/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:51:36 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 15:48:42 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
// #include "utils.hpp"
// #include "webserv.hpp"
class Route
{
public:
    enum RouteType {
        FILE,
        DIRECTORY,
        CGI,
        MAPPED,
        OTHER
    };
    
    Route();
    Route(std::string const & path);
    Route(std::string const & root, std::string const & path, RouteType routeType);
    ~Route();

    // Getters
    std::string                                 getPath() const;
    std::string                                 getRoot() const;
    std::vector<std::string>                    getIndex() const;
    std::string                                 getRedirect() const;
    std::vector<std::string>                    getMethods() const;
    std::string                                 getUploadDir() const;
    bool                                        getAllowListing() const;
    std::map<int, std::string>                  getErrorPages() const;
    std::string                                 getCgiPath() const;
    std::vector<std::string>                    getCgiExt() const;

    // Setters
    void                                        setPath(const std::string& path);
    void                                        setRoot(const std::string& root);
    void                                        setIndex(const std::vector<std::string>& index);
    void                                        setRedirect(const std::string& redirect);
    void                                        setMethods(const std::vector<std::string>& methods);
    void                                        setUploadDir(const std::string& uploadDir);
    void                                        setAllowListing(const bool& allowListing);
    void                                        setErrorPages(const std::map<int, std::string>&);
    void                                        setCgiPath(const std::string& cgiPath);
    void                                        setCgiExt(const std::vector<std::string>& cgiExt);

    std::string getFullPath() const {
        return fullPath;
    }

    void setFullPath(std::string const &path) {
        fullPath = path;
    }

    RouteType getRouteType() const {
        return routeType;
    }

    void setRouteType(RouteType type) {
        routeType = type;
    }

    // Methods
    void                        print() const;
    void                        fill(std::string const &line, int &lineNb);
private:
    std::string                             path;
    std::string                             root;
    std::vector<std::string>                index;
    std::string                             redirect;
    std::vector<std::string>                methods;
    std::string                             uploadDir;
    std::map<int, std::string>              errorPages;
    bool                                    allowListing;
    std::vector<std::string>                cgiExt;
    std::string                             cgiPath;

    RouteType                               routeType;
    std::string                             fullPath;

};


