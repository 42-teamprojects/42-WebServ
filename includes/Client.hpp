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
	size_t total_read;
	std::string response;
	size_t total_send;
	std::string method;
	bool chunked;
	size_t startCunter;
	size_t content_length;
public:
	Client() : socket(-1), buffer(""), total_read(0), response(""), total_send(0),method("") , chunked(false), startCunter(0),content_length(0)  {};


	~Client();
	Client(Client const& src);
	Client &operator=(Client const& src);

	size_t getcontent_length()
	{
		return this->content_length;
	}

	void setcontent_length(size_t content_length)
	{
		this->content_length = content_length;
	}

	void setstartCunter(size_t startCunter)
	{
		this->startCunter = startCunter;
	}
	size_t getstartCunter()
	{
		return this->startCunter;
	}

	void setchunked(bool chunked)
	{
		this->chunked = chunked;
	}
	bool getchunked()
	{
		return this->chunked;
	}

	std::string getmethod()
	{
		return this->method;
	}
	void setmethod(std::string method)
	{
		this->method = method;
	}
	void setappendbuffer(std::string tmp)
	{
		this->buffer.append(tmp);
	}
	Client(int socket);
	Client(std::string response, int read);
	int getSocket() const;
	void add_to_total_read(size_t read);
	size_t getTotalRead() const;
	std::string getBuffer() const;
	void setBuffer(std::string buffer);
	void setSocket(int socket);
	void setResponse(std::string response){
		this->response = response;
	}
	std::string getResponse() const{
		return this->response;
	}
	void add_to_total_send(size_t send){
		this->total_send += send;
	}
	size_t getTotalSend() const{
		return this->total_send;
	}
	void reset()
	{
		this->total_send = 0;
		this->total_read = 0;
		this->buffer = "";
		this->response = "";
		this->method = "";
		this->socket = -1;
		this->startCunter = 0;
		this->content_length = 0;
		this->chunked = false;
	}


};

#endif
