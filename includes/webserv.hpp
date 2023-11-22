/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:35:49 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/22 12:59:32 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <fcntl.h>
#include <vector>
#include <map>
#include <stack>
#include <sys/stat.h>
#include <dirent.h>

#include "utils.hpp"
#include "Console.hpp"

#include "Server.hpp"
#include "Route.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "WebServer.hpp"
#include "ServerException.hpp"

# define DEFAULT_ROOT "/var/www/html"
