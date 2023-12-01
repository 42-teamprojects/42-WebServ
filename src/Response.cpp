/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:56:24 by yelaissa          #+#    #+#             */
/*   Updated: 2023/12/01 11:46:31 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "utils.hpp"

Response::Response(std::string const & buffer) {
    mimes = Mimes();
    isListing = false;
    request = new Request(buffer);
    code = OK;
    handleResponse();
}

Response::~Response() {
    delete request;
}

Route Response::deepSearch(Server & server, std::string const & resource) {
    std::vector<Route> routes = server.getRoutes();
    for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); it++) {
        std::pair<std::string, bool>   matchedPath = getMatchedPath(it->getPath(), resource);
        if (matchedPath.second) {
            std::string newResource = "/" + matchedPath.first;
            removeConsecutiveChars(newResource, '/');
            Route route = findBestMatch(*it, newResource);
            checkRedirection(*it);
            checkMethods(*it);
            return route;
        }
    }
    return findBestMatch(server, resource); 
}

Route Response::getRoute(Server & server) {
    std::string resource = getRequestedResource(request->getUri());
    
    // Get matched route for request
    std::vector<Route>::iterator it = server.find(resource);
    if (it == server.end()) {
        return deepSearch(server, resource);
    }
    if (resource.back() != '/') {
        headers["Location"] = request->getUri() + "/";
        throw ServerException(MovedPermanently);
    }
    checkRedirection(*it);
    checkMethods(*it);
    return *it;
}

std::string Response::getFilePath(Server const & server, Route const & route) {
    if (route.getRouteType() == Route::FILE)
        return route.getRoot() + "/" + route.getPath();
    
    try {
        std::string root, index;
        root = route.getRoot().empty() ? server.getRoot() : route.getRoot();
        index = tryFiles(server, route, root);
        return root + "/" + index;
    } catch (ServerException & e) {
        // check if listing is allowed
        if (e.getCode() == Forbidden && (route.getAllowListing() || server.getAllowListing()) && request->getMethod() == "GET") {
            std::vector<std::string> files = getFilesInDirectory(route.getRoot(), route.getPath());
            return (isListing = true, body = generateHtmlListing(files), "");
        }
        throw ServerException(e.getCode());
    }
}

void Response::handleGet(Server const & server, Route const & route) {
    std::string filePath = getFilePath(server, route);
    if (isListing)
        return;
    removeConsecutiveChars(filePath, '/');
    headers["Content-Type"] = mimes[getFileExt(filePath)];
    if (!route.getCgiPath().empty()) {
        Console::info("Serving CGI file: " + filePath);
        Cgi cgi(route.getCgiPath(), filePath, *request);
        body = cgi.getResponseBody();
        return; 
    }
    Console::info("Serving file: " + filePath);
    readFile(filePath, OK);
}

void Response::handleDelete(Server const & server, Route const & route) {
    std::string filePath = getFilePath(server, route);
    removeConsecutiveChars(filePath, '/');
    if (!route.getCgiPath().empty()) {
        Console::info("Running CGI: " + filePath);
        Cgi cgi(route.getCgiPath(), filePath, *request);
        body = cgi.getResponseBody();
        return; 
    }
    
}

/* 
TODO:
    - body max size
 */
void Response::handleResponse() {
    Server server = getServer();
    try {
        Route route = getRoute(server);
        if (request->getMethod() == "GET") {
            handleGet(server, route);
        }
        else if (request->getMethod() == "POST") {
            std::cout << "POST" << std::endl;
        }
        else if (request->getMethod() == "DELETE") {
            handleDelete(server, route);
        }
        else {
            throw ServerException(NotImplemented);
        }
    } catch (ServerException & e) {
        code = e.getCode();
        readFile(server.getErrorPages()[code], code);
        Console::warning(request->getUri() + " : " + getStatusMessage(code));
    } catch (std::exception & e) {
        code = ServerError;
        readFile(server.getErrorPages()[code], code);
        Console::error(e.what());
    }
}