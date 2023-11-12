/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:42:11 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/08 20:01:36 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#include "webserv.hpp"

Route::Route() : path(""), root(""), index(), redirect(), methods(), uploadDir(""), allowListing(0), cgiExt(), cgiPath("") {};
Route::~Route() {};

// Getters
std::string Route::getPath() const {
    return path;
}
std::string Route::getRoot() const {
    return root;
}
std::vector<std::string> Route::getIndex() const {
    return index;
}
std::string Route::getRedirect() const {
    return redirect;
}
std::vector<std::string> Route::getMethods() const {
    return methods;
}
std::string Route::getUploadDir() const {
    return uploadDir;
}
bool Route::getAllowListing() const {
    return allowListing;
}
std::string Route::getCgiPath() const {
    return cgiPath;
}
std::vector<std::string> Route::getCgiExt() const {
    return cgiExt;
}

// Setters
void Route::setPath(const std::string& path) {
    this->path = path;
}
void Route::setRoot(const std::string& root) {
    this->root = root;
}
void Route::setIndex(const std::vector<std::string>& index) {
    this->index = index;
}
void Route::setRedirect(const std::string& redirect) {
    this->redirect = redirect;
}
void Route::setMethods(const std::vector<std::string>& methods) {
    this->methods = methods;
}
void Route::setUploadDir(const std::string& uploadDir) {
    this->uploadDir = uploadDir;
}
void Route::setAllowListing(const bool& allowListing) {
    this->allowListing = allowListing;
}
void Route::setCgiPath(const std::string& cgiPath) {
    this->cgiPath = cgiPath;
}
void Route::setCgiExt(const std::vector<std::string>& cgiExt) {
    this->cgiExt = cgiExt;
}

// Methods
void                        Route::print() const
{
    if (!path.empty())
        std::cout << "path: " << path << std::endl;
    if (!root.empty())
        std::cout << "root: " << root << std::endl;
    if (!index.empty()) {
        std::cout << "index: ";
        printContainer(index);
    }
    if (!redirect.empty())
        std::cout << "redirect: " << redirect << std::endl;
    if (!methods.empty()) {
        std::cout << "methods: ";
        printContainer(methods);
    }
    if (!uploadDir.empty())
        std::cout << "uploadDir: " << uploadDir << std::endl;
    if (allowListing)
        std::cout << "allowListing: " << allowListing << std::endl;
    if (!cgiPath.empty())
        std::cout << "cgiPath: " << cgiPath << std::endl;
    if (!cgiExt.empty()) {
        std::cout << "cgiExt: ";
        printContainer(cgiExt);
    }
}

void                        Route::fill(std::string const &line, int &lineNb)
{
    std::vector<std::string>    split;
    std::string                 option, value;

    split = ft_split<std::vector<std::string> >(line, "=");

    std::string commonOptions[] = {"error_page", "client_max_body_size", "autoindex", "root", "index", "allow_methods"};
    
    if (split.size() != 2)
        throw ServerException("Invalid server line", lineNb);
    option = split[0];
    value = split[1];
    if (value.empty() || option.empty())
        throw ServerException("Invalid server line", lineNb);
    if (option == "path") {
        setPath(value);
    }
    else if (option == "root") {
        setRoot(value);
    }
    else if (option == "index") {
        std::vector<std::string>    index = ft_split<std::vector<std::string> >(value, ", ");
        if (index.empty())
            throw ServerException("Invalid server line", lineNb);
        setIndex(index);
    }
    else if (option == "redirect") {
        setRedirect(value);
    }
    else if (option == "methods") {
        std::vector<std::string>    methods = ft_split<std::vector<std::string> >(value, ", ");
        if (methods.empty())
            throw ServerException("Invalid server line", lineNb);
        setMethods(methods);
    }
    else if (option == "upload_dir") {
        setUploadDir(value);
    }
    else if (option == "allow_listing") {
        if (value == "on" || value == "1" || value == "true")
            setAllowListing(true);
        else if (value == "off" || value == "0" || value == "false")
            setAllowListing(false);
        else
            throw ServerException("Invalid server line", lineNb);
    }
    else if (option == "cgi_path") {
        setCgiPath(value);
    }
    else if (option == "cgi_ext" || option == "cgi_extension") {
        std::vector<std::string>    cgiExt = ft_split<std::vector<std::string> >(value, ", ");
        if (cgiExt.empty())
            throw ServerException("Invalid server line", lineNb);
        setCgiExt(cgiExt);
    }
    else
        std::cout << "Invalid route option: " << line << std::endl;
        // throw ServerException("Invalid server option", lineNb);
}