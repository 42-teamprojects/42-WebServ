/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:17:59 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/03 14:46:57 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include "ServerContext.hpp"

class ConfigFile
{
private:
    std::vector<ServerContext>         servers;
public:
    ConfigFile();
    ~ConfigFile();
};