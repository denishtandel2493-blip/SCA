#include <iostream>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <unistd.h>

std::vector<std::string> tasks = {
    "Learn Docker",
    "Learn Kubernetes",
    "Deploy Application"
};

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Task Manager API running on port 8080" << std::endl;

    while (true) {
        int client = accept(server_fd, nullptr, nullptr);

        std::string body;

        for (size_t i = 0; i < tasks.size(); i++) {
            body += std::to_string(i + 1) + ". " + tasks[i] + "\n";
        }

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n\r\n" +
            body;

        send(client, response.c_str(), response.size(), 0);
        close(client);
    }

    return 0;
}
