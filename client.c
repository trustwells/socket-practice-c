#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
int main()
{

    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Need to setup up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. Connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // 4. Send and receive
    send(sock_fd, "Hello from client!", strlen("Hello from client!"), 0);
    recv(sock_fd, buffer, sizeof(buffer), 0);
    printf("Received: %s\n", buffer);

    // 5. Cleanup
    close(sock_fd);

    return 0;
}