/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:18:52 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/06 18:13:40 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"

class Cgi
{
	private :
		Route		cgiRoute;
		std::string filename;
		std::string responseBody;
		std::map<std::string, std::string> env;
		char **envp;
	public :
		Cgi();
		Cgi(Route const & cgiRoute, std::string const & filename, Request const & req);
		Cgi(Cgi const& other);
		Cgi& operator=(Cgi const& other);
		~Cgi();
		std::string getResponseBody();
		void executCgi(Request const & req);
};
