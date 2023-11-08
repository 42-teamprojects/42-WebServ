/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:29:33 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 22:47:30 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

class Location;

class Context
{
protected:
    std::vector<std::string>    errorPage;
    size_t                      clientMaxBodySize;
    std::string                 root;
    std::vector<std::string>    index;
    bool                        autoIndex;
    std::vector<std::string>    allowMethods;
    std::vector<Location*>       locations;
public:
    Context();
    ~Context();

    // Getters
    std::vector<std::string>    getErrorPage() const;
    size_t                      getClientMaxBodySize() const;
    std::string                 getRoot() const;
    std::vector<std::string>    getIndex() const;
    bool                        getAutoIndex() const;
    std::vector<std::string>    getAllowMethods() const;
    std::vector<Location*>       getLocations() const;

    // Setters
    void                        setErrorPage(const std::vector<std::string>& errorPage);
    void                        setClientMaxBodySize(size_t clientMaxBodySize);
    void                        setRoot(const std::string& root);
    void                        setIndex(const std::vector<std::string>& index);
    void                        setAutoIndex(bool autoIndex);
    void                        setAllowMethods(const std::vector<std::string>& allowMethods);
    void                        setLocations(const std::vector<Location*>& locations);
    void                        addLocation(Location *location);

    // Methods
    void                printCommon() const;
    void                fillCommon(std::string &, std::string &, int &);
};