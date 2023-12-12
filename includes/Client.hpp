/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htalhaou <htalhaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 13:22:47 by htalhaou          #+#    #+#             */
/*   Updated: 2023/12/10 16:32:23 by htalhaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
# define Client_HPP

# include "webserv.hpp"
# include "utils.hpp"
# include "WebServer.hpp"


class Client
{
private:
	int socket;
	std::string buffer;
public:
	Client();
	~Client();
	Client(Client const& src);
	Client &operator=(Client const& src);

	Client(int socket);
	
	int getSocket() const;
	std::string getBuffer();
	void setBuffer(std::string buffer);
	void setSocket(int socket);
};

#endif
