#pragma once

class ConnectionInfo
{
public:
	ConnectionInfo() : socket() {}
	ConnectionInfo(int a) : socket(a) {}
	int socket;
};
