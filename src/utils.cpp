/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 21:19:31 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/21 22:24:36 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::vector<std::string> getFilesInDirectory(std::string const & directoryPath) {
    std::vector<std::string> files;

    DIR* dir = opendir(directoryPath.c_str());
    if (dir == NULL) {
        Console::error("Error opening directory: " + directoryPath);
        throw ServerException(ServerError);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (entry->d_type == DT_DIR)
                files.push_back(entry->d_name + std::string("/"));
            else
                files.push_back(entry->d_name);
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