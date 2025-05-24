#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {0};
    socklen_t addr_len = sizeof(client_addr);

    // step 1 create a socket, if returning -1 then print error
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind to IP and port, use IPv4, any IP address, and convert host port to network version in binary (big endian)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen, allow up to 5 connections in the queue
    if (listen(server_fd, 5) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening for a connection...\n");

    // 4. Accept a connection

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0)
    {
        perror("Error accepting client connection");
        exit(EXIT_FAILURE);
    }

    printf("Client is connected\n");

    // 5. Receive and send
    recv(client_fd, buffer, sizeof(buffer), 0);
    printf("Received: %s\n", buffer);

    const char *response = "Hello from server!";

    send(client_fd, response, strlen(response), 0);

    // 6. Cleanup
    close(client_fd);
    close(server_fd);

    return 0;
}
