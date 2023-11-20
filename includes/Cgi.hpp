/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:18:52 by htalhaou          #+#    #+#             */
/*   Updated: 2023/11/19 14:21:41 by htalhaou         ###   ########.fr       */
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
	public :
		Cgi();
		Cgi(std::string path, std::string filename);
		Cgi(Cgi const& other);
		Cgi& operator=(Cgi const& other);
		~Cgi();
		void setPath(std::string path);
		void setFilename(std::string filename);
		std::string getPath();
		std::string getFilename();
		void executCgi(Request &req, Response &res, Server &serv);
};
