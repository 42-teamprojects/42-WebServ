/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:29:33 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 22:47:30 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Location;

// Abstract class
class CommonDirectives
{
protected:
    std::vector<std::string>    errorPage;
    size_t                      clientMaxBodySize;
    std::string                 root;
    std::vector<std::string>    index;
    bool                        autoIndex;
    std::vector<std::string>    allowMethods;
    std::vector<Location>       locations;
public:
    CommonDirectives() {};
    virtual ~CommonDirectives() {};

    // Getters
    std::vector<std::string>    getErrorPage() const { return errorPage; }
    size_t                      getClientMaxBodySize() const { return clientMaxBodySize; }
    std::string                 getRoot() const { return root; }
    std::vector<std::string>    getIndex() const { return index; }
    bool                        getAutoIndex() const { return autoIndex; }
    std::vector<std::string>    getAllowMethods() const { return allowMethods; }
    std::vector<Location>       getLocations() const { return locations; }

    // Setters
    void                        setErrorPage(const std::vector<std::string>& errorPage) { this->errorPage = errorPage; }
    void                        setClientMaxBodySize(size_t clientMaxBodySize) { this->clientMaxBodySize = clientMaxBodySize; }
    void                        setRoot(const std::string& root) { this->root = root; }
    void                        setIndex(const std::vector<std::string>& index) { this->index = index; }
    void                        setAutoIndex(bool autoIndex) { this->autoIndex = autoIndex; }
    void                        setAllowMethods(const std::vector<std::string>& allowMethods) { this->allowMethods = allowMethods; }
    void                        setLocations(const std::vector<Location>& locations) { this->locations = locations; }
    void                        addLocation(const Location& location) { this->locations.push_back(location); }

    // Methods
    virtual void                print() const = 0;
    virtual void                fill(std::string const &line, int &lineNb) = 0;
};