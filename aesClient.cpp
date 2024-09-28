#include <openssl/aes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
#define AES_KEY_SIZE 16 // AES key size in bytes
int main()
{
	int clientSock;
	char buffer[BUFFER_SIZE];
	sockaddr_in serverAddr;

	//socket creation	
	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	//connect
	connect(clientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	
	//sending message
	printf("Enter message: ");	
	fgets(buffer, BUFFER_SIZE, stdin);
	printf("\n");
	AES_KEY key;
	const unsigned char aesKey[] = "1234567890123456"; // 16-byte AES key
	AES_set_encrypt_key(aesKey, AES_KEY_SIZE * 8, &key);
	AES_encrypt((const unsigned char *)buffer, (unsigned char *)buffer, &key);	
	printf("Encrypted message: ");
	for (int i = 0; i < strlen(buffer); i++) {
		printf("%02x", (unsigned char)buffer[i]);
	}
	printf("\n");

	//send
	send(clientSock, buffer, strlen(buffer), 0);
	close(clientSock);
	return 0;
}
