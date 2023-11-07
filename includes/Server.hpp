/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 13:51:41 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/05 14:15:27 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Server : public Context
{
private:
    size_t                          listen;
    std::string                     host;
    std::vector<std::string>        serverName;
public:
    Server();
    ~Server();

    // Getters
    size_t                      getListen() const;
    std::string                 getHost() const;
    std::vector<std::string>    getServerName() const;

    // Setters
    void                        setListen(const size_t &);
    void                        setHost(const std::string &);
    void                        setServerName(const std::vector<std::string>&);

    // Methods
    void                        print() const;
    void                        fill(std::string const &, int &);
};
    