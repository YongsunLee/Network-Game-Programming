#pragma once

class CNetwork {
public:
	CNetwork();
	~CNetwork();

	void err_quit(char *msg);
	void err_display(char *msg);
	int recvn(SOCKET s, char *buf, int len, int flags);

private:
	CTimer m_timer;
	
	CRITICAL_SECTION cs;
	list<SOCKET> sockList;
	list<ClientMsg*>clientDatabuf;

	SOCKET listen_sock;

	ServerMsg m_Msg;
};

DWORD WINAPI ServerRecv(LPVOID arg);
DWORD WINAPI ServerSend(LPVOID arg);