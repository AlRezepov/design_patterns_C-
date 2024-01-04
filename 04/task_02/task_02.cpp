#include <fstream>
#include <stdexcept>

class Formatter {
public:
    virtual std::string format(const std::string& data) const = 0;
};

class Saver {
public:
    virtual ~Saver() = default;
    virtual void saveTo(std::ofstream& file, const std::string& data) const = 0;
};

class HTMLFormatter : public Formatter {
public:
    std::string format(const std::string& data) const override {
        return "<html>" + data + "<html/>";
    }
};

class JSONFormatter : public Formatter {
public:
    std::string format(const std::string& data) const override {
        return "{ \"data\": \"" + data + "\"}";
    }
};

class TextFormatter : public Formatter {
public:
    std::string format(const std::string& data) const override {
        return data;
    }
};

class FileSaver : public Saver {
private:
    const Formatter& formatter;

public:
    explicit FileSaver(const Formatter& formatter) : formatter(formatter) {}

    void saveTo(std::ofstream& file, const std::string& data) const override {
        file << formatter.format(data);
    }
};

class Data {
private:
    std::string data;

public:
    explicit Data(std::string data) : data(std::move(data)) {}

    const std::string& getData() const {
        return data;
    }
};

int main() {
    Data data("Sample Data");

    HTMLFormatter htmlFormatter;
    JSONFormatter jsonFormatter;
    TextFormatter textFormatter;

    FileSaver htmlSaver(htmlFormatter);
    FileSaver jsonSaver(jsonFormatter);
    FileSaver textSaver(textFormatter);

    std::ofstream htmlFile("output.html");
    std::ofstream jsonFile("output.json");
    std::ofstream textFile("output.txt");

    htmlSaver.saveTo(htmlFile, data.getData());
    jsonSaver.saveTo(jsonFile, data.getData());
    textSaver.saveTo(textFile, data.getData());

    htmlFile.close();
    jsonFile.close();
    textFile.close();

    return 0;
}
