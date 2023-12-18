#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>

class SqlSelectQueryBuilder {
public:
    
    SqlSelectQueryBuilder() {}
    
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns.emplace_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        table_name = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        where_conditions.emplace_back(column + "=" + value);
        return *this;
    }

    std::string BuildQuery() const {
        std::ostringstream query;

        query << "SELECT ";
        if (columns.empty()) {
            query << "*";
        }
        else {
            for (size_t i = 0; i < columns.size(); ++i) {
                query << columns[i];
                if (i < columns.size() - 1) {
                    query << ", ";
                }
            }
        }

        query << " FROM " << table_name;

        if (!where_conditions.empty()) {
            query << " WHERE ";
            for (size_t i = 0; i < where_conditions.size(); ++i) {
                query << where_conditions[i];
                if (i < where_conditions.size() - 1) {
                    query << " AND ";
                }
            }
        }

        query << ";";

        return query.str();
    }

private:
    std::vector<std::string> columns;
    std::string table_name;
    std::vector<std::string> where_conditions;
};

int main() {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    assert(
        query_builder.BuildQuery() == "SELECT name, phone FROM students WHERE id=42 AND name=John;"
    );

    std::cout << "Query: " << query_builder.BuildQuery() << std::endl;

    return 0;
}
