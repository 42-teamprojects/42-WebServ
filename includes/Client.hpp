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
	int total_read;
	std::string response;
	int total_send;
public:
	Client();


	~Client();
	Client(Client const& src);
	Client &operator=(Client const& src);

	Client(int socket);
	Client(std::string response, int read);
	int getSocket() const;
	void add_to_total_read(int read);
	int getTotalRead() const;
	std::string getBuffer() const;
	void setBuffer(std::string buffer);
	void setSocket(int socket);
	void setResponse(std::string response){
		this->response = response;
	}
	std::string getResponse() const{
		return this->response;
	}
	void add_to_total_send(int send){
		this->total_send += send;
	}
	int getTotalSend() const{
		return this->total_send;
	}


};

#endif
