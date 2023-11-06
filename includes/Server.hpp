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
#include "Context.hpp"

class Server : public Context
{
private:
    std::string                 listen;
    std::string                 host;
    std::vector<std::string>    serverName;

public:
    Server();
    ~Server();

    // Getters
    std::string                 getListen() const;
    std::string                 getHost() const;
    std::vector<std::string>    getServerName() const;

    // Setters
    void                        setListen(const std::string& listen);
    void                        setHost(const std::string& host);
    void                        setServerName(const std::vector<std::string>& serverName);

    // Methods
    void                        print() const;
    void                        fill(std::string const &line, int &lineNb);
};
    