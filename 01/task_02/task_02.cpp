#include <iostream>
#include <vector>
#include <sstream>
#include <map>

class SqlSelectQueryBuilder {
public:
    SqlSelectQueryBuilder() {}

    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& newColumns) noexcept {
        columns = newColumns;
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        fromTable = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        whereConditions.push_back({ column, value });
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& pair : kv) {
            whereConditions.push_back(pair);
        }
        return *this;
    }

    std::string BuildQuery() const {
        std::stringstream query;

        query << "SELECT ";
        if (columns.empty()) {
            query << "*";
        }
        else {
            for (const auto& column : columns) {
                query << column << ", ";
            }
            query.seekp(-2, std::ios_base::end);
        }

        query << " FROM " << fromTable;

        if (!whereConditions.empty()) {
            query << " WHERE ";
            for (const auto& condition : whereConditions) {
                query << condition.first << "=" << condition.second << " AND ";
            }
            query.seekp(-5, std::ios_base::end);
        }

        query << ";";

        return query.str();
    }

private:
    std::vector<std::string> columns;
    std::string fromTable;
    std::vector<std::pair<std::string, std::string>> whereConditions;
};


int main() {
    SqlSelectQueryBuilder query_builder;

    query_builder.AddColumns({ "name", "phone", "email" });

    query_builder.AddWhere({ {"id", "42"}, {"name", "John"} });

    query_builder.AddFrom("students");

    std::cout << "Query: " << query_builder.BuildQuery() << std::endl;

    return 0;
}
