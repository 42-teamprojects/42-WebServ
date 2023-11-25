/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:19:31 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/25 16:32:53 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::vector<std::string> getFilesInDirectory(std::string const & rootPath, std::string const & reqPath, bool isRoute) {
    std::vector<std::string> files;
    std::string              directoryPath = rootPath + "/" + (!isRoute ? reqPath : "");

    DIR* dir = opendir(directoryPath.c_str());
    if (dir == NULL) {
        Console::error("Error opening directory: " + directoryPath);
        throw ServerException(NotFound);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::string path = reqPath + "/" + entry->d_name;
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
    std::string htmlFilePath = "/tmp/.listing.html";

    std::ofstream htmlFile(htmlFilePath);

    if (!htmlFile.is_open()) {
        Console::error("Opening HTML file for writing listings");
        return NULL;
    }
    
    htmlFile << "<!DOCTYPE html>\n<html>\n<head>\n<title>File Listing</title>\n</head>\n<body>\n";
    htmlFile << "<ul>\n";
    for (size_t i = 0; i < files.size(); ++i) {
        htmlFile << "  <li><a href='" << files[i] << "'>" << files[i] << "</a></li>\n";
    }
    htmlFile << "</ul>\n";
    htmlFile << "</body>\n</html>\n";
    htmlFile.close();

    return htmlFilePath;
}

bool    mapErrorPages(std::map<int, std::string> & errorPages, std::string const & value)
{
    std::vector<std::string>    pages = ft_split(value, ", ");
    for (std::vector<std::string>::iterator it = pages.begin(); it != pages.end(); ++it) {
        std::vector<std::string>    page = ft_split(*it, ":");
        if (page.size() != 2)
            return false;
        int code = std::atoi(page[0].c_str());
        if (code < 300 || code > 599)
            return false;
        errorPages[code] = page[1];
    }
    if (pages.empty())
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

bool isPathMatched(std::string serverRootPath, std::string path) {
// Remove trailing slashes from server root path and path
    while (!serverRootPath.empty() && serverRootPath.back() == '/')
        serverRootPath.pop_back();
    while (!path.empty() && path.back() == '/')
        path.pop_back();

    // Check if the server root path is a prefix of the path
    if (path.find(serverRootPath) == 0) {
        // Check if the next character after the server root path is a slash or the end of the string
        if (path.length() == serverRootPath.length() || path[serverRootPath.length()] == '/')
            return true; // Return true if the path matches the server root path
    }
    return false; // Return false if the path doesn't match the server root path
}

std::string getMatchedPath(std::string serverRootPath, std::string path) {
    // Remove trailing slashes from server root path and path
    while (!serverRootPath.empty() && serverRootPath.back() == '/')
        serverRootPath.pop_back();
    while (!path.empty() && path.back() == '/')
        path.pop_back();

    // Check if the server root path is a prefix of the path
    if (path.find(serverRootPath) == 0) {
        // Check if the next character after the server root path is a slash or the end of the string
        if (path.length() == serverRootPath.length() || path[serverRootPath.length()] == '/')
            return path.substr(serverRootPath.length() + 1); // Return the new path excluding the root path
    }
    return path; // Return the original path if it doesn't match the server root path
}
