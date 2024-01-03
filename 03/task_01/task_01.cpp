#include <iostream>
#include <fstream>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << "Console Log: " << message << std::endl;
    }
};

class FileLogCommand : public LogCommand {
private:
    std::string filePath;

public:
    FileLogCommand(const std::string& path) : filePath(path) {}

    void print(const std::string& message) override {
        std::ofstream outFile(filePath, std::ios::app);
        if (outFile.is_open()) {
            outFile << "File Log: " << message << std::endl;
            outFile.close();
        }
        else {
            std::cerr << "Unable to open file: " << filePath << std::endl;
        }
    }
};

void print(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main() {
    ConsoleLogCommand consoleLog;
    FileLogCommand fileLog("log.txt");

    print(consoleLog, "Hello, Console!");
    print(fileLog, "Hello, File!");

    return 0;
}
