#include "Connector.hpp"

Connector::Connector() { }

Connector::~Connector() { }

int Connector::getSockFd(int idxServer) {
	return _servSockFd[idxServer];
}

int Connector::makeConnection(const std::string& portNum, int idxServer) {
	_servSockFd[idxServer] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_servSockFd[idxServer] == CONNECTION_FAIL) {
		Debug::log("socket fd error");
		return CONNECTION_FAIL;
	}
	_servAddr[idxServer].sin_family = AF_INET;
	_servAddr[idxServer].sin_addr.s_addr = htonl(INADDR_ANY);
	_servAddr[idxServer].sin_port = htons(stoi(portNum));
	if (bind(_servSockFd[idxServer], (struct sockaddr*) &_servAddr[idxServer], sizeof(_servAddr[idxServer])) == CONNECTION_FAIL) {
		Debug::log("bind error " + portNum);
		return CONNECTION_FAIL;
	}
	if (listen(_servSockFd[idxServer], 1024)) {
		Debug::log("listen error " + portNum);
		return CONNECTION_FAIL;
	}
	fcntl(_servSockFd[idxServer], F_SETFL, O_NONBLOCK);
	return CONNECTION_OK;
}

void Connector::closeConnection(int idxServer) {
	close(_servSockFd[idxServer]);
}
