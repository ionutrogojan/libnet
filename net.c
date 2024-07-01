#include "net.h"

int net_resolve_addr(struct net_conn *conn, SocketType type, const u16 port)
{
	conn->addr_len = sizeof(struct net_address);

	if ((conn->fd = syscall(NET_SYS_SOCKET, NET_IPV4, type, NET_PROTO_AUTO)) < 0)
	{
		// perror("Socket creation error\n");
		return conn->fd;
	}

	int opt = 1;

	if (syscall(NET_SYS_SETSOCKOPT, conn->fd, NET_SOCKET_OPT, NET_OPT_REUSEADDR, &opt, sizeof(&opt)) < 0)
	{
		// perror("Socket SETSOCKOPT REUSEADDR failed");
		net_close(conn->fd);
		return -1;
	}

	if (syscall(NET_SYS_SETSOCKOPT, conn->fd, NET_SOCKET_OPT, NET_OPT_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		// perror("Socket SETSOCKOPT REUSEPORT failed");
		net_close(conn->fd);
		return -1;
	}

	conn->address.family = NET_IPV4;
	conn->address.addr.address = NET_ADDRIN_ANY;
	
	union { int i; char c; } u = { 1 };
	if (u.c)
	{
		conn->address.port = ( port<<8 | port>>8 );
	}
	else
	{
		conn->address.port = port;
	}

	if (syscall(NET_SYS_BIND, conn->fd, (struct net_address *)&conn->address, conn->addr_len) < 0)
	{
		// perror("Socket address binding failed");
		net_close(conn->fd);
		return -1;
	}

	return 0;
}

int net_listen(struct net_conn *conn, int backlog)
{
	return syscall(NET_SYS_LISTEN, conn->fd, backlog);
}

int net_accept(struct net_conn *conn)
{
	return syscall(NET_SYS_ACCEPT, conn->fd, &conn->address, &conn->addr_len);
}

int net_read(int fd, char *buf, unsigned long count)
{
	return syscall(NET_SYS_READ, fd, buf, count);
}

int net_close(unsigned int fd)
{
	return syscall(NET_SYS_CLOSE, fd);
}