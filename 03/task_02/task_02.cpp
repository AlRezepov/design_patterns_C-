#include <iostream>
#include <fstream>
#include <vector>

// Интерфейс наблюдателя
class Observer {
public:
    virtual void onWarning(const std::string& message) = 0;
    virtual void onError(const std::string& message) = 0;
    virtual void onFatalError(const std::string& message) = 0;
};

// Наблюдаемый класс
class Logger {
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void warning(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onWarning(message);
        }
    }

    void error(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onError(message);
        }
    }

    void fatalError(const std::string& message) const {
        for (Observer* observer : observers) {
            observer->onFatalError(message);
        }
    }
};

class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
    void onError(const std::string& message) override {}

    void onFatalError(const std::string& message) override {}
};

class ErrorObserver : public Observer {
private:
    std::ofstream outFile;

public:
    ErrorObserver(const std::string& filePath) {
        outFile.open(filePath, std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Unable to open file: " << filePath << std::endl;
        }
    }

    ~ErrorObserver() {
        outFile.close();
    }

    void onError(const std::string& message) override {
        std::cout << "Error: " << message << std::endl;
        if (outFile.is_open()) {
            outFile << "Error: " << message << std::endl;
        }
    }

    void onWarning(const std::string& message) override {}

    void onFatalError(const std::string& message) override {}
};

class FatalErrorObserver : public Observer {
private:
    std::ofstream outFile;

public:
    FatalErrorObserver(const std::string& filePath) {
        outFile.open(filePath, std::ios::app);
        if (!outFile.is_open()) {
            std::cerr << "Unable to open file: " << filePath << std::endl;
        }
    }

    ~FatalErrorObserver() {
        outFile.close();
    }

    void onFatalError(const std::string& message) override {
        std::cout << "Fatal Error: " << message << std::endl;
        if (outFile.is_open()) {
            outFile << "Fatal Error: " << message << std::endl;
        }
    }

    void onWarning(const std::string& message) override {}

    void onError(const std::string& message) override {}
};

int main() {
    Logger logger;

    WarningObserver warningObserver;
    ErrorObserver errorObserver("error_log.txt");
    FatalErrorObserver fatalErrorObserver("fatal_error_log.txt");

    logger.addObserver(&warningObserver);
    logger.addObserver(&errorObserver);
    logger.addObserver(&fatalErrorObserver);

    // Генерация событий
    logger.warning("This is a warning.");
    logger.error("This is an error.");
    logger.fatalError("This is a fatal error.");

    return 0;
}
