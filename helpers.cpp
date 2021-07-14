// preluat din laborator 10 PC si modificat + DIE preluat din laborator 7

#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "buffer.cpp"

using namespace std;

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while(0)

#define BUFLEN 4096
#define LINELEN 1000

#define WELCOME "--------------------Introduceti comanda--------------------"
#define CONNECT "--------------------Conectare la server--------------------"
#define FINALMS "--------------------Termina text server--------------------"
#define EXITCLI "--------------------Inchidere interfata--------------------"
#define LOGOUTM "--------------------Delogare din server--------------------"
#define STARTER "Comenzi: <register> <login> <logout> <exit> <enter_library>"

#define PORT 8080
#define HOST "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define REGISTER "/api/v1/tema/auth/register"
#define LOGIN "/api/v1/tema/auth/login"
#define ACCESS "/api/v1/tema/library/access"
#define BOOKS "/api/v1/tema/library/books"
#define LOGOUT "/api/v1/tema/auth/logout"
#define HOST_IP "3.8.116.10"

char *compute_get_request(string host, string url, string cookie, string token) {
    string message = "GET " + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n";
		if (!cookie.empty())
			message = message + "Cookie: " + cookie + "\r\n";
		if (!token.empty())
    	message = message + "Authorization: Bearer" + token + "\r\n" +"\r\n";
    char* c = new char[message.length() + 1];
    strcpy(c, message.c_str());
    return c;
}

char *compute_post_request(string host, string url, string cookie, string token, string body) {
	  string message = "POST " + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n";
		if (!cookie.empty())
			message = message + "Cookie: " + cookie + "\r\n";
		if (!token.empty())
    	message = message + "Authorization: Bearer" + token + "\r\n" +"\r\n";
    message = message + "Content-Type: application/json\r\n";
    message = message + "Content-Length: " + to_string(body.length()) + "\r\n\r\n";
    message = message + body + "\r\n\r\n";
    char* c = new char[message.length() + 1];
    strcpy(c, message.c_str());
    return c;
}

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void compute_message(char *message, const char *line) {
    strcat(message, line);
    strcat(message, "\r\n");
}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag) {
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
      error("ERROR opening socket");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
      error("ERROR connecting");
    return sockfd;
}

void close_connection(int sockfd) {
    close(sockfd);
}

void send_to_server(int sockfd, char *message) {
    int bytes, sent = 0;
    int total = strlen(message);
    do {
      bytes = write(sockfd, message + sent, total - sent);
      DIE(bytes < 0, "send.\n");
      sent += bytes;
    } while (bytes && sent < total);
}

char *receive_from_server(int sockfd) {
    int bytes, received = 0, total = BUFLEN;
    char *response = (char*)calloc(BUFLEN, sizeof(char));
    DIE(response == NULL, "calloc\n");
    do {
      bytes = read(sockfd, response + received, total - received);
      DIE(bytes < 0, "read.\n");
      received += bytes;
    } while (bytes && received < total);
    DIE(received == total, "recv\n");
    return response;
}

char *basic_extract_json_response(char *str) {
    return strstr(str, "{\"");
}
