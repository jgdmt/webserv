/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vilibert <vilibert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:34:54 by vilibert          #+#    #+#             */
/*   Updated: 2024/06/12 16:17:10 by vilibert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <iostream>
Socket::Socket(Server &serv)
{
    (void)serv;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr.s_addr);
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);
    // sockaddr clientAddress;
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    std::cout << "Connection accepted" << std::endl;
    while(1)
    {
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string buf = buffer;
    if(buf.size() > 1)
    std::cout << "Message from client: " << buffer << std::endl;
    }
}