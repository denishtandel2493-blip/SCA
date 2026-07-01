#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct Task {
    int id;
    std::string title;
    bool done;
};

std::string dbFile() {
    const char* env = std::getenv("APP_DB_FILE");
    return env ? std::string(env) : std::string("tasks.db");
}

std::vector<Task> loadTasks() {
    std::vector<Task> tasks;
    std::ifstream in(dbFile());
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, done, title;
        std::getline(ss, id, '|');
        std::getline(ss, done, '|');
        std::getline(ss, title);
        tasks.push_back({ std::stoi(id), title, done == "1" });
    }
    return tasks;
}

void saveTasks(const std::vector<Task>& tasks) {
    std::ofstream out(dbFile(), std::ios::trunc);
    for (const auto& task : tasks) {
        out << task.id << '|' << (task.done ? 1 : 0) << '|' << task.title << "\n";
    }
}

int nextId(const std::vector<Task>& tasks) {
    int maxId = 0;
    for (const auto& t : tasks) maxId = std::max(maxId, t.id);
    return maxId + 1;
}

void printBanner() {
    std::cout << "\n=== DevOps Task Runner ===\n";
    std::cout << "Console app for Docker and Kubernetes practice\n\n";
}

void showMenu() {
    std::cout << "1. List tasks\n";
    std::cout << "2. Add task\n";
    std::cout << "3. Complete task\n";
    std::cout << "4. Delete task\n";
    std::cout << "5. Run simulated batch job\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose: ";
}

void listTasks(const std::vector<Task>& tasks) {
    if (tasks.empty()) {
        std::cout << "No tasks found.\n";
        return;
    }
    std::cout << "\nCurrent tasks:\n";
    for (const auto& task : tasks) {
        std::cout << "[" << (task.done ? 'x' : ' ') << "] "
            << task.id << " - " << task.title << "\n";
    }
}

void addTask(std::vector<Task>& tasks) {
    std::cout << "Enter task title: ";
    std::string title;
    std::getline(std::cin >> std::ws, title);
    tasks.push_back({ nextId(tasks), title, false });
    saveTasks(tasks);
    std::cout << "Task added.\n";
}

void completeTask(std::vector<Task>& tasks) {
    std::cout << "Enter task id to complete: ";
    int id;
    std::cin >> id;
    for (auto& task : tasks) {
        if (task.id == id) {
            task.done = true;
            saveTasks(tasks);
            std::cout << "Task completed.\n";
            return;
        }
    }
    std::cout << "Task not found.\n";
}

void deleteTask(std::vector<Task>& tasks) {
    std::cout << "Enter task id to delete: ";
    int id;
    std::cin >> id;
    auto before = tasks.size();
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
        [id](const Task& t) { return t.id == id; }), tasks.end());
    if (tasks.size() != before) {
        saveTasks(tasks);
        std::cout << "Task deleted.\n";
    }
    else {
        std::cout << "Task not found.\n";
    }
}

void runJob() {
    std::cout << "Running simulated batch job";
    for (int i = 0; i < 5; ++i) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::time_t now = std::time(nullptr);
    std::cout << " done at " << std::put_time(std::localtime(&now), "%F %T") << "\n";
}

int main() {
    std::vector<Task> tasks = loadTasks();

    while (true) {
        printBanner();
        showMenu();

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: listTasks(tasks); break;
        case 2: addTask(tasks); break;
        case 3: completeTask(tasks); break;
        case 4: deleteTask(tasks); break;
        case 5: runJob(); break;
        case 0:
            std::cout << "Bye.\n";
            return 0;
        default:
            std::cout << "Invalid option.\n";
            break;
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}
