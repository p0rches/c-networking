#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, World!";

  // Creating Socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    return -1;
  }

  // Forcefully attaching socket to the port 8080
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    return -1;
  }

  // Start listening for the connections on the socket
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    return -1;
  }

  printf("PORT = %d\n", PORT);

  while (1) {
    // Accept a new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      continue; // Continue to accept further connections
    }

    // Read data sent by the client
    read(new_socket, buffer, 1024);
    printf("Received request: %s\n", buffer);

    // Send response to client
    write(new_socket, hello, strlen(hello));

    // Close the connection
    close(new_socket);
  }

  return 0;
}
