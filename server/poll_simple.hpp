
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
};
struct pollfd poll_fd_detailed(size_t file_descriptor, size_t timeout)
{
	struct pollfd fd;
	fd.fd = file_descriptor;
	fd.events = POLLIN | POLL_ERR;
	poll(&fd, 1, timeout);
	return fd;
}
//returns true if there is a packet on the socket
bool poll_fd(size_t file_descriptor, size_t timeout)
{
	return poll_fd_detailed(file_descriptor, timeout).revents & POLLIN;
}
bool is_valid_fd(size_t file_descriptor)
{
	return poll_fd_detailed(file_descriptor, 0).revents & POLL_ERR;
}