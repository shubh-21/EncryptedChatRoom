#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 1024
#define AES_KEY_SIZE 16 // AES key size in bytes
int main()
{
	int sockfd, newSocket;
	char buffer[BUFFER_SIZE];
	sockaddr_in serverAddr, cliAddr;
	socklen_t addr_size;
	
	//Socket Creation
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	//binding
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ;
	
	//listening
	listen(sockfd, 10);
	printf("Listening for incoming connections...\n\n");
	addr_size = sizeof(cliAddr);
	
	//accept
	newSocket = accept(sockfd, (struct sockaddr *)&cliAddr, &addr_size);
	
	//receiving message
	ssize_t bytesRead = recv(newSocket, buffer, BUFFER_SIZE, 0);
	printf("Encrypted message received from client: ");

	for (int i = 0; i < bytesRead; i++) {
		printf("%02x", (unsigned char)buffer[i]);
	}
	printf("\n");

	AES_KEY key;
	const unsigned char aesKey[] = "1234567890123456"; // 16-byte AES key
	AES_set_decrypt_key(aesKey, AES_KEY_SIZE * 8, &key);
	AES_decrypt((const unsigned char *)buffer, (unsigned char *)buffer, &key);
	// Display decrypted data
	printf("Response from client: %s\n", buffer);
	// Close sockets
	close(newSocket);
	close(sockfd);
	return 0;
}	
