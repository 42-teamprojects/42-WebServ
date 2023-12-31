/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:19:31 by yelaissa          #+#    #+#             */
/*   Updated: 2023/12/06 18:09:26 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::vector<std::string> getFilesInDirectory(std::string const & rootPath, std::string const & reqPath) {
    std::vector<std::string> files;
    std::string              directoryPath = rootPath + "/";

    DIR* dir = opendir(directoryPath.c_str());
    if (dir == NULL) {
        Console::error("Error opening directory: " + directoryPath);
        throw ServerException(NotFound);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string dirName = std::string(entry->d_name);
        if (dirName != "." && dirName !=  "..") {
            std::string path = reqPath + "/" + dirName;
            removeConsecutiveChars(path, '/');
            if (entry->d_type == DT_DIR)
                files.push_back(path + std::string("/"));
            else
                files.push_back(path);
        }
    }
    closedir(dir);
    return files;
}

std::string generateHtmlListing(const std::vector<std::string>& files) {

    std::string html;

    html += "<!DOCTYPE html>\n<html>\n<head>\n<title>Directory Listing</title>\n</head>\n<body>\n";
    html += "<h1 style='padding-left:20px'>Directory Listing</h1>\n";
    html += "<ul>\n";
    for (size_t i = 0; i < files.size(); ++i) {
        html += "  <li style='font-size:20px'><a href='" + files[i] + "'>" + files[i] + "</a></li>\n";
    }
    html += "</ul>\n";
    html += "</body>\n</html>\n";

    return html;
}

bool    mapErrorPages(std::map<int, std::string> & errorPages, std::string const & value)
{
    std::vector<std::string>    pages = ft_split(value, ", ");
    for (std::vector<std::string>::iterator it = pages.begin(); it != pages.end(); ++it) {
        std::vector<std::string>    page = ft_split(*it, ":");
        if (page.size() != 2)
            return false;
        int code = std::atoi(page[0].c_str());
        if (code < 100 || code > 599)
            return false;
        errorPages[code] = page[1];
    }
    if (pages.empty())
        return false;
    return true;
}

bool mapCgi(std::map<std::string, std::string> & cgi, std::string const & value)
{
    std::vector<std::string>    cgiVals = ft_split(value, ", ");
    for (std::vector<std::string>::iterator it = cgiVals.begin(); it != cgiVals.end(); ++it) {
        std::vector<std::string>    page = ft_split(*it, ":");
        if (page.size() != 2)
            return false;
        cgi[page[0]] = page[1];
    }
    if (cgiVals.empty())
        return false;
    return true;
}

bool isDirectory(std::string path)
{
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0)
        return false;

    return S_ISDIR(stat_buf.st_mode);
}

bool isFile(std::string path)
{
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0)
        return false;

    return S_ISREG(stat_buf.st_mode);
}


void removeConsecutiveChars(std::string & s, char c)
{
    std::string result;
    std::string::iterator it;
    
    for (it = s.begin(); it != s.end(); ++it)
    {
        if (result.empty() || *it != c || result.back() != c)
        {
            result.push_back(*it);
        }
    }
    s = result;
}


void trim(std::string & s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos)
        s.clear();
    else
        s = s.substr(start, end - start + 1);
}

std::vector<std::string> ft_split(const std::string &s, const std::string &delimiters)
{
    std::vector<std::string> splited;
    size_t i = 0;

    while (i < s.length())
    {
        i = s.find_first_not_of(delimiters, i);
        
        if (i == std::string::npos)
            break;
        size_t wordEnd = s.find_first_of(delimiters, i);
        
        std::string word = s.substr(i, wordEnd - i);
        trim(word);
        splited.push_back(word);
        
        i = wordEnd;
    }
    return splited;
}

void trimTrailingSlashes(std::string & s)
{
    while (!s.empty() && s.back() == '/')
        s.pop_back();
}

std::pair<std::string, bool>    getMatchedPath(std::string serverRootPath, std::string path) {
    std::pair<std::string, bool> value;
    value.second = false;
    trimTrailingSlashes(serverRootPath);       
    if (path.find(serverRootPath) == 0) {
        if (path.length() == serverRootPath.length() || path[serverRootPath.length()] == '/') {
            value.first = path.substr(serverRootPath.length());
            value.second = true;
        } 
    }
    else
        value.first = path;
    return value;
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter)
{
    std::vector<std::string> splitted;
    size_t start = 0, end = 0;

    while ((end = s.find(delimiter, start)) != std::string::npos)
    {
        std::string token = s.substr(start, end - start);
        trim(token);

        if (!token.empty())
            splitted.push_back(token);

        start = end + delimiter.length();
    }

    std::string lastToken = s.substr(start);
    trim(lastToken);

    if (!lastToken.empty())
        splitted.push_back(lastToken);

    return splitted;
}

void removeFile(std::string path) {
    if (std::remove(path.c_str()) != 0) {
        Console::error("Error removing file " + path);
        throw ServerException(ServerError);
    } else {
        Console::info("File " + path + " successfully removed");
    }
}

void removeDirectory(std::string path) {
    DIR* dir = opendir(path.c_str());

    if (!dir) {
        Console::error("Error opening directory " + path);
        throw ServerException(ServerError);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string dirName = std::string(entry->d_name);
        if (dirName != "." && dirName !=  "..") {
            std::string fullPath = path + "/" + dirName;
            removeConsecutiveChars(fullPath, '/');

            if (isFile(fullPath)) {
                removeFile(fullPath);
            } else if (isDirectory(fullPath)) {
                removeDirectory(fullPath);
            }
        }
    }
    closedir(dir);
}

void removeFileOrDirectory(std::string path) {
    if (isFile(path)) {
        removeFile(path);
        throw ServerException(NoContent);
    } else if (isDirectory(path)) {
        removeDirectory(path);
        throw ServerException(NoContent);
    } else {
        Console::error("Error removing file or directory " + path);
        throw ServerException(NotFound);
    }
}
