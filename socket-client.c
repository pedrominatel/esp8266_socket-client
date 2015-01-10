/*
 * socket-client.c
 *
 *  Created on: Jan 10, 2015
 *      Author: Pedro Minatel
 *
 *      This socket client was based on the example published on
 *      http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
 *
 *      The firmware on the ESP8266 is the:
 *      http://blog.electrodragon.com/esp8266-gpio-test-edited-firmware/
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "socket-client.h"

void error(char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]) {

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 3) {
		fprintf(stderr, "ESP8266 Socket Test Usage: %s Hostname Port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(argv[1]);

	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR connecting");

	while(1){
		n = write(sockfd, "setgpio_10", 10);
		sleep(1);
		n = write(sockfd, "setgpio_01", 10);
		sleep(1);

		if (n < 0) {
			error("ERROR writing to socket");
			break;
		}
	}

	fprintf(stderr, "ERROR, no such host\n");

	return 0;
}
