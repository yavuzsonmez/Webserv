#include "../../inc/network/ServerSocket.hpp"
#include "../../inc/network/ClientSocket.hpp"
#include "../../inc/http/Response.hpp"

ServerSocket::ServerSocket(unsigned short port, unsigned int address)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0); //IPv4, TCP
	if (_fd < 0)
		throw SocketCreationError();

	_socket.sin_family = AF_INET;
	_socket.sin_port = htons(port); //host byte order to network byte order
	_socket.sin_addr.s_addr = address;
	bzero(&(_socket.sin_zero), 8);

	if (bind(_fd, (struct sockaddr *)&_socket, sizeof(struct sockaddr_in)))
		throw SocketCreationError();

	if (listen(_fd, BACKLOG))
		throw SocketCreationError();

	processConnections();
}

ServerSocket::~ServerSocket(){}

int ServerSocket::getFileDescriptor() const { return _fd; }

void ServerSocket::processConnections()
{
	int forward;
	struct sockaddr_in clientSocket;
	socklen_t socketSize = sizeof(struct sockaddr_in);

	while (1)
	{
		forward = accept(_fd, (struct sockaddr *)&clientSocket, &socketSize);
		std::cout << "Request" << std::endl;

		// if accept return -1 throw error
		ClientSocket client(clientSocket);
		Response response;
		std::string httpResponse(response.getResponse());

		int bytes_send;
		bytes_send = 0;
		// if bytes_send < len we have to handle it and keep send until everything was sent
		while (bytes_send < (int)httpResponse.length())
		{
			//if (bytes_send == -1)
				// if send return -1, throw error; + perror
			bytes_send = send(forward, httpResponse.c_str(), httpResponse.length(), 0);
			httpResponse.erase(0, bytes_send);
		}

		close(forward);
		//if close(server) //throw error; + perror
	}
}