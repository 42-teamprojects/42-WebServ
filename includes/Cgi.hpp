/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:18:52 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/30 15:44:39 by yelaissa         ###   ########.fr       */
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
		std::string path;
		std::string filename;
		std::string responseBody;
		std::map<std::string, std::string> env;
		char **envp;
	public :
		Cgi();
		Cgi(std::string const & path, std::string const & filename, Request const & req);
		Cgi(Cgi const& other);
		Cgi& operator=(Cgi const& other);
		~Cgi();
		void setPath(std::string path);
		void setFilename(std::string filename);
		std::string getPath();
		std::string getFilename();
		std::string getResponseBody();
		void executCgi();
};
