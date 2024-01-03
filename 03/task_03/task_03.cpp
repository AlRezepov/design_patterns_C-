#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>

enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
public:
    LogMessage(Type type, const std::string& message) : messageType(type), logMessage(message) {}

    Type type() const {
        return messageType;
    }

    const std::string& message() const {
        return logMessage;
    }

private:
    Type messageType;
    std::string logMessage;
};

class Handler {
public:
    virtual void handle(const LogMessage& message) = 0;
    virtual ~Handler() = default;
};

class FatalErrorHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::FatalError) {
            throw std::runtime_error(message.message());
        }
    }
};

class ErrorHandler : public Handler {
private:
    std::ofstream outFile;

public:
    ErrorHandler(const std::string& filePath) {
        outFile.open(filePath, std::ios::app);
        if (!outFile.is_open()) {
            throw std::runtime_error("Unable to open file: " + filePath);
        }
    }

    ~ErrorHandler() override {
        outFile.close();
    }

    void handle(const LogMessage& message) override {
        if (message.type() == Type::Error) {
            if (outFile.is_open()) {
                outFile << "Error: " << message.message() << std::endl;
            }
        }
    }
};

class WarningHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::Warning) {
            std::cout << "Warning: " << message.message() << std::endl;
        }
    }
};

class UnknownHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::Unknown) {
            throw std::runtime_error("Unknown message: " + message.message());
        }
    }
};

class LogHandlerChain {
private:
    std::unique_ptr<Handler> fatalHandler;
    std::unique_ptr<Handler> errorHandler;
    std::unique_ptr<Handler> warningHandler;
    std::unique_ptr<Handler> unknownHandler;

public:
    LogHandlerChain(const std::string& errorFilePath)
        : fatalHandler(std::make_unique<FatalErrorHandler>()),
        errorHandler(std::make_unique<ErrorHandler>(errorFilePath)),
        warningHandler(std::make_unique<WarningHandler>()),
        unknownHandler(std::make_unique<UnknownHandler>()) {}

    void process(const LogMessage& message) {
        try {
            fatalHandler->handle(message);
            errorHandler->handle(message);
            warningHandler->handle(message);
            unknownHandler->handle(message);
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }
    }
};

int main() {
    LogHandlerChain handlerChain("error_log.txt");

    LogMessage warningMessage(Type::Warning, "This is a warning.");
    LogMessage errorMessage(Type::Error, "This is an error.");
    LogMessage fatalErrorMessage(Type::FatalError, "This is a fatal error.");
    LogMessage unknownMessage(Type::Unknown, "This is an unknown message.");

    handlerChain.process(warningMessage);
    handlerChain.process(errorMessage);
    handlerChain.process(fatalErrorMessage);
    handlerChain.process(unknownMessage);

    return 0;
}
