/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Console.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 19:42:39 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/19 20:11:40 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iostream>
#include <ctime>
#include <string>

# define INFO 0
# define WARNING 1
# define ERROR 2
# define DEBUG 3

class Console {
public:
    static void info(const std::string& message) {
        log(INFO, message);
    }

    static void warning(const std::string& message) {
        log(WARNING, message);
    }

    static void error(const std::string& message) {
        log(ERROR, message);
    }

    static void debug(const std::string& message) {
        log(DEBUG, message);
    }

    static void log(int level, const std::string& message) {
        std::string logMessage = getCurrentTime() + " " + getLabel(level) + " " + message;
        std::cout << logMessage << std::endl;
    }


    static void ping() {
        std::cout << "PING" << std::endl;
    }

private:
    Console() {}
    ~Console() {}

    static std::string getCurrentTime() {
        std::time_t now = std::time(NULL);
        char buf[100] = {0};
        std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
        return buf;
    }

    static std::string getLabel(int level) {
        switch (level) {
            case 0: return "\033[1;32m[INFO]\033[0m";
            case 1: return "\033[1;33m[WARNING]\033[0m";
            case 2: return "\033[1;31m[ERROR]\033[0m";
            case 3: return "\033[1;34m[DEBUG]\033[0m";
            default: return "";
        }
    }
};