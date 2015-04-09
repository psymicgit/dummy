/*==================================================================================================
 * 项目名称:网络模块
 *    功能:收发网络数据
 *    作者: 
 *    联系: 
 *    版本: v1.0.0
 ==================================================================================================*/

#include "syssocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>

#define BACKLOG 50

/*********************************************************************************************
 *函   数   名：sysutil_activate_keepalive
 *功        能：发送保持活动包。
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_keepalive(int fd) {
	int keepalive = 1;
	int retval = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive,
			sizeof(keepalive));
	if (retval != 0) {
		perror("setsockopt: keepalive");
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_activate_reuseaddr
 *功        能：允许套接口和一个已在使用中的地址捆绑
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_reuseaddr(int fd) {
	int reuseaddr = 1;
	int retval = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
			sizeof(reuseaddr));
	if (retval != 0) {
		perror("setsockopt: reuseaddr");
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_set_nodelay
 *功        能：禁止发送合并的Nagle算法
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_set_nodelay(int fd) {
	int nodelay = 1;
	int retval = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay,
			sizeof(nodelay));
	if (retval != 0) {
		perror("setsockopt: nodelay");
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_activate_oobinline
 *功        能：在常规数据流中接收带外数据
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_oobinline(int fd) {
	int oob_inline = 1;
	int retval = setsockopt(fd, SOL_SOCKET, SO_OOBINLINE, &oob_inline,
			sizeof(oob_inline));
	if (retval != 0) {
		perror("setsockopt: oobinline");
	}
}
/*********************************************************************************************
 *函   数   名：sysutil_set_iptos_throughput
 *功        能：在IP头中设置选项
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_set_iptos_throughput(int fd) {
	int tos = IPTOS_THROUGHPUT;
	/* Ignore failure to set (maybe this IP stack demands privilege for this) */
	(void) setsockopt(fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
}
/*********************************************************************************************
 *函   数   名：sysutil_activate_linger
 *功        能：如关闭时有未发送数据，则逗留
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_linger(int fd) {
	int retval;
	struct linger the_linger;
	memset(&the_linger, 0x00, sizeof(the_linger));
	the_linger.l_onoff = 1;
	the_linger.l_linger = 32767;
	retval = setsockopt(fd, SOL_SOCKET, SO_LINGER, &the_linger,
			sizeof(the_linger));
	if (retval != 0) {
		perror("setsockopt: linger");
	}
}
/*********************************************************************************************
 *函   数   名：sysutil_deactivate_linger_failok
 *功        能：如关闭时有未发送数据，则逗留
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_deactivate_linger_failok(int fd) {
	struct linger the_linger;
	the_linger.l_onoff = 0;
	the_linger.l_linger = 0;
	(void) setsockopt(fd, SOL_SOCKET, SO_LINGER, &the_linger,
			sizeof(the_linger));
}

/*********************************************************************************************
 *函   数   名：sysutil_activate_sigurg
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_sigurg(int fd) {
	int retval = fcntl(fd, F_SETOWN, getpid());
	if (retval != 0) {
		perror("fcntl");
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_activate_noblock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_noblock(int fd) {
	int retval;
	int curr_flags = fcntl(fd, F_GETFL);
	if (curr_flags < 0) {
		perror("fcntl");
	}
	curr_flags |= O_NONBLOCK;
	retval = fcntl(fd, F_SETFL, curr_flags);
	if (retval != 0) {
		perror("fcntl");
	}
}
/*********************************************************************************************
 *函   数   名：sysutil_deactivate_noblock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_deactivate_noblock(int fd) {
	int retval;
	int curr_flags = fcntl(fd, F_GETFL);
	if (curr_flags < 0) {
		perror("fcntl");
	}
	curr_flags &= ~O_NONBLOCK;
	retval = fcntl(fd, F_SETFL, curr_flags);
	if (retval != 0) {
		perror("fcntl");
	}
}
/*********************************************************************************************
 *函   数   名：sysutil_recv_peek
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_recv_peek(const int fd, void* p_buf, unsigned int len) {
	while (1) {
		int retval = recv(fd, p_buf, len, MSG_PEEK);
		if (retval < 0) {
			continue;
		}
		return retval;
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_get_ipv4_sock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_get_ipv4_sock(void) {
	int retval = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (retval < 0) {
		perror("sysutil_get_ipv4_sock");
	}
	return retval;
}

/*********************************************************************************************
 *函   数   名：sysutil_get_ipv6_sock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_bind(int fd, const int port) {
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	return bind(fd, (struct sockaddr*) &addr, sizeof(addr));
}

/*********************************************************************************************
 *函   数   名：sysutil_get_ipv6_sock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_listen(int fd, const unsigned int backlog) {
	return listen(fd, backlog);
}


/*********************************************************************************************
 *函   数   sysutil_dns_resolve
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_dns_resolve(char *ip, const unsigned int max, const char * host)
{
	struct hostent* hent = (struct hostent*)gethostbyname(host); //返回对应于给定主机名的包含主机名字和地址信息的hostent结构指针
	if (hent != NULL){
		strcpy(ip, inet_ntoa(*(struct in_addr *)hent->h_addr_list[0]));
		//strcpy(ip, inet_ntoa((unsigned int *)hent->h_addr_list[0]));
	}
	else{
		strcat(ip, host);
	}
	return 0;
}

/*********************************************************************************************
 *函   数   名：sysutil_accept_timeout
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_accept_timeout(int fd, struct sockaddr_in* p_sockaddr,
		unsigned int wait_seconds) {
	int retval;
	int saved_errno;
	fd_set accept_fdset;
	struct timeval timeout;
	unsigned int socklen = sizeof(struct sockaddr_in);
	if (wait_seconds > 0) {
		FD_ZERO(&accept_fdset);
		FD_SET(fd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do {
			retval = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
			saved_errno = errno;
		} while (retval < 0 && saved_errno == EINTR);
		if (retval == 0) {
			errno = EAGAIN;
			return -1;
		}
	}
	retval = accept(fd, (struct sockaddr*) p_sockaddr, &socklen);
	return retval;
}

/*********************************************************************************************
 *函   数   名：sysutil_connect_timeout
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_connect_timeout(int fd, const char* ip, const unsigned int port,
		unsigned int wait_seconds) {
	unsigned int addrlen = 0;
	int retval;
	int saved_errno;

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = inet_addr(ip);
	addrlen = sizeof(sockaddr);
	if (wait_seconds > 0) {
		sysutil_activate_noblock(fd);
	}
	retval = connect(fd, (struct sockaddr*) &sockaddr, addrlen);
	if (retval < 0 && errno == EINPROGRESS) {
		fd_set connect_fdset;
		struct timeval timeout;
		FD_ZERO(&connect_fdset);
		FD_SET(fd, &connect_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do {
			retval = select(fd + 1, NULL, &connect_fdset, NULL, &timeout);
			saved_errno = errno;
		} while (retval < 0 && saved_errno == EINTR);
		if (retval == 0) {
			retval = -1;
			errno = EAGAIN;
		} else {
			socklen_t socklen = sizeof(retval);
			int sockoptret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &retval,
					&socklen);
			if (sockoptret != 0 || retval) {
				retval = -1;
			}
		}
	}
	if (wait_seconds > 0) {
		sysutil_deactivate_noblock(fd);
	}
	return retval;
}

/*********************************************************************************************
 *函   数   名：sysutil_read
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_read(const int fd, void* p_buf, const unsigned int size) {
	while (1) {
		int retval = read(fd, p_buf, size);
		int saved_errno = errno;
		if (retval < 0 && saved_errno == EINTR) {
			continue;
		}
		return retval;
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_read_timeout
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_read_timeout(const int fd, void* p_buf, const unsigned int size,
		unsigned int wait_seconds) {
	//if (wait_seconds > 0) {
	//	sysutil_activate_noblock(fd);
	//}
	fd_set fdset;
	struct timeval timeout;
	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);
	timeout.tv_sec = wait_seconds;
	timeout.tv_usec = 0;

	int retval = select(fd + 1, NULL, &fdset, NULL, &timeout);
	if ((retval > 0) && (FD_ISSET(fd, &fdset))) {
		printf("*******************\n");
		retval = sysutil_read(fd, p_buf, size);
	} else {
		printf("*******************\n");
		retval = -1;
	}
	//if (wait_seconds > 0) {
	//	sysutil_deactivate_noblock(fd);
	//}
	return retval;
}

/*********************************************************************************************
 *函   数   名：sysutil_write
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_write(const int fd, const void* p_buf, const unsigned int size) {
	while (1) {
		int retval = write(fd, p_buf, size);
		int saved_errno = errno;
		if (retval < 0 && saved_errno == EINTR) {
			continue;
		}
		return retval;
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_read_loop
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_read_loop(const int fd, void* p_buf, unsigned int size) {
	int retval;
	int num_read = 0;
	while (1) {
		retval = sysutil_read(fd, (char*) p_buf + num_read, size);
		if (retval < 0) {
			return retval;
		} else if (retval == 0) {
			/* Read all we're going to read.. */
			return num_read;
		}
		if ((unsigned int) retval > size) {
			perror("retval too big in sysutil_read_loop");
			return -1;
		}
		num_read += retval;
		size -= (unsigned int) retval;
		if (size == 0) {
			/* Hit the read target, cool. */
			return num_read;
		}
	}
}

/*********************************************************************************************
 *函   数   名：sysutil_write_loop
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_write_loop(const int fd, const void* p_buf, unsigned int size) {
	int retval;
	int num_written = 0;
	while (1) {
		retval = sysutil_write(fd, (const char*) p_buf + num_written, size
				- num_written);
		if (retval < 0) {
			/* Error */
			return retval;
		} else if (retval == 0) {
			/* Written all we're going to write.. */
			return num_written;
		}
		if ((unsigned int) retval > size) {
			perror("retval too big in sysutil_read_loop");
			return -1;
		}
		num_written += retval;
		size -= (unsigned int) retval;
		if (size == 0) {
			/* Hit the write target, cool. */
			return num_written;
		}
	}
}

