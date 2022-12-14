#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
  // Créez un socket pour écouter les connexions entrantes
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create socket" << std::endl;
    return 1;
  }

  // Configurez l'adresse et le port sur lesquels le serveur écoutera les connexions
  struct sockaddr_in server_addr;
  std::memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(80);

  // Associez l'adresse et le port au socket
  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Failed to bind socket to address/port" << std::endl;
    return 1;
  }

  // Mettez le socket en mode d'écoute pour les connexions entrantes
  if (listen(server_fd, 10) < 0) {
    std::cerr << "Failed to listen on socket" << std::endl;
    return 1;
  }

  // Attendez une connexion entrante
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
  if (client_fd < 0) {
    std::cerr << "Failed to accept incoming connection" << std::endl;
    return 1;
  }

  // Recevez les données envoyées par le client
  char buffer[1024];
  ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
  if (bytes_received < 0) {
    std::cerr << "Failed to receive data from client" << std::endl;
    return 1;
  }

  // Affichez les données reçues à la console
  std::cout << "Received data from client: " << buffer << std::endl;

  // Fermez la connexion avec le client
  close(client_fd);

  // Fermez le socket d'écoute
  close(server_fd);
  return 0;
}
