/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:37:30 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/29 12:42:11 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

// Get date GMT format
std::string getDateGMT();

// Get file Extension
std::string getFileExt(std::string const &path);

// Has access
bool hasAccess(std::string const &path);