/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:35:49 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/16 16:13:54 by msodor           ###   ########.fr       */
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

#include "utils.hpp"

#include "Server.hpp"
#include "Route.hpp"
#include "Request.hpp"
#include "WebServer.hpp"
#include "Config.hpp"
#include "ServerException.hpp"
#include "Response.hpp"
