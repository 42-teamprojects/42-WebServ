/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientClass.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 13:22:47 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/10 14:17:38 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTCLASS_HPP
# define CLIENTCLASS_HPP

# include "webserv.hpp"
# include "utils.hpp"
# include "WebServer.hpp"


class ClientClass
{
private:
	int socket;
	std::string buffer;
public:
	ClientClass();
	~ClientClass();
	ClientClass(ClientClass const& src);
	ClientClass &operator=(ClientClass const& src);

	ClientClass(int socket);
	
	int getSocket() const;
	std::string getBuffer();
	void setBuffer(std::string buffer);
	void setSocket(int socket);
};

#endif
