/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 22:42:11 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 22:49:51 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {};
Location::~Location() {};

// Getters
std::string                 Location::getPath() const { return path; }
std::string                 Location::getRedir() const { return redir; }

// Setters
void                        Location::setPath(const std::string& path) { this->path = path; }
void                        Location::setRedir(const std::string& redir) { this->redir = redir; }

// Methods
void                        Location::print() const
{

}
void                        Location::fill(std::string const &line, int &lineNb)
{
    (void) line;
    (void) lineNb;
}