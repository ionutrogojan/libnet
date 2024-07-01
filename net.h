#ifndef NET_H
#define NET_H

#define NET_SYS_READ 0
#define NET_SYS_CLOSE 3
#define NET_SYS_SOCKET 41
#define NET_SYS_ACCEPT 43
#define NET_SYS_BIND 49
#define NET_SYS_LISTEN 50
#define NET_SYS_SETSOCKOPT 54

#define NET_SOCKET_OPT 1

#define NET_OPT_REUSEADDR 2
#define NET_OPT_REUSEPORT 15

#define NET_ADDRIN_ANY 0

typedef unsigned short int u16;
typedef unsigned int u32;

struct address
{
	u32 address;
};

struct socket
{
	u16 family;	/* Address family and length  */
	char data[14];	/* Address data */
};

struct net_address
{
	u16 family;	/* Address family */
	u16 port;	/* Port number */
	struct address addr;	/* Internet address */
	unsigned char zero
	[
		sizeof(struct socket) - 
		sizeof(u16) - 
		sizeof(u16) - 
		sizeof(struct address)
	];	/* Pad to size of `struct socket' */
};

struct net_conn
{
	int fd;
	struct net_address address;
	int addr_len;
	const u16 port;
};

typedef enum
{
	NET_TCP = 1,
	NET_UDP = 2
}
SocketType;

typedef enum
{
	NET_IPV4 = 2,
	NET_IPV6 = 10
}
AddressFamily;

typedef enum
{
	NET_PROTO_AUTO = 0,
	NET_PROTO_TCP = 6,
	NET_PROTO_UDP = 17
}
ProtocolType;

#ifndef UNISTD_H
	#include <unistd.h>
#endif // UNISTD_H

int net_resolve_addr(struct net_conn *conn, SocketType type, const u16 port);

int net_listen(struct net_conn *conn, int backlog);

int net_accept(struct net_conn *conn);

int net_read(int fd, char *buf, unsigned long count);

int net_close(unsigned int fd);

#endif // NET_H