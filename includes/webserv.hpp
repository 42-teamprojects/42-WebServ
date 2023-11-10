/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:35:49 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/10 20:08:02 by yelaissa         ###   ########.fr       */
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

#include <vector>
#include <map>
#include <stack>

#include "utils.hpp"

#include "Route.hpp"
#include "Server.hpp"
#include "ConfigParser.hpp"
#include "ServerException.hpp"
