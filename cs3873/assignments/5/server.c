// Nicholas Simmons srz888

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define TARGET_PORT 51019

void sig_chld(int signo);

int main(void) {
	struct sockaddr_in from;
	struct sockaddr_in to;
	char buffer[100];
	char *response = "Message Received";
	int sock, set, status, l, flags, size;
	int Q_SIZE, new_sock, pid, setOptSize, nbytes;
	socklen_t addrlen;

	// Create the socket
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		fprintf(stderr, "Socket creation error: %d\n", sock);
		return -1;
	}

	// Set socket options
	setOptSize = 1;
	set = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &setOptSize, sizeof(setOptSize));
	if (set < 0) {
		fprintf(stderr, "Setting socket options error: %d\n", set);
		return -1;
	}

	// Bind the socket
	status = bind(sock, (struct sockaddr *) &to, sizeof(to));
	if (status < 0) {
		fprintf(stderr, "Bind Result: %d\n", status);
		return -1;
	}

	// Listen on the port
	l = listen(sock, Q_SIZE);
	if (l < 0) {
		fprintf(stderr, "Listen Error: %d\n", l);
		return -1;
	}

	signal(SIGCHLD, sig_chld);

	// Accept the connection
	while(1) {
		addrlen = sizeof(from);
		new_sock = accept(new_sock, (struct sockaddr *) &from, &addrlen);
		if (new_sock < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				printf("Accept\n");
				return -1;
			}
		}

		// Fork the process
		pid = fork( );
		if (pid == 0) {
			nbytes = 99;
			flags = 0;
			close(sock);
			size = recv(new_sock, buffer, nbytes, flags);

			if (size < 0) {
				fprintf(stderr, "Recieving Error: %d\n", size);
				return -1;
			}

			flags = 0;
			size = send(new_sock, response, strlen(response), flags);
			close(new_sock);
			return 0;
		} else {
			if (pid == -1) {
				fprintf(stderr, "Forking Error: %d\n", pid);
				return -1;
			}

			close(new_sock);
		}
	}
}

void sig_chld(int signo) {
	pid_t pid;
	int stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0);
	return;
}
