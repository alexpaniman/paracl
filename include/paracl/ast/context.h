#pragma once

#include <unordered_map>
#include <string>


namespace paracl {

class context {
public:
    void create_variable(const std::string &name) {
        variables_[name] = 0;
    }

    bool check_var_existing(std::string &name) {
        if (variables_.find(name) != variables_.end()) {
            return true;
        }
        return false;
    }

    int64_t *get_variable(std::string &name) {
        if (variables_.find(name) == variables_.end()) {
            //обработка
        }
        return &(variables_[name]);
    }

private:
    std::unordered_map<std::string, int64_t> variables_{};
};

} // end namespace paracl