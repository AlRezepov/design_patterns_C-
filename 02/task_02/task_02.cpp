#include <map>
#include <string>
#include <iostream>

class VeryHeavyDatabase {
public:
    virtual std::string GetData(const std::string& key) {
        return "Very Big Data String: " + key;
    }
};

class OneShotDB : public VeryHeavyDatabase {
public:
    OneShotDB(VeryHeavyDatabase* real_object, size_t shots = 1)
        : real_db_(real_object), remaining_shots_(shots) {}

    std::string GetData(const std::string& key) override {
        if (remaining_shots_ > 0) {
            --remaining_shots_;
            return real_db_->GetData(key);
        }
        else {
            return "error";
        }
    }

private:
    VeryHeavyDatabase* real_db_;
    size_t remaining_shots_;
};

int main() {
    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);

    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;

    return 0;
}

