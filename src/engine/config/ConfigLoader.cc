#include "ConfigLoader.hpp"

#include <string>
#include <exception>

ConfigLoader::ConfigLoader(std::string path) : path(path) {
    this->file.open(path);

    while (!file.eof()) {
        std::string input;

        getline(this->file, input);

        auto sp = this->loadConfigPair(input);
        this->data.push_back(sp);
    }
}

std::tuple<std::string, std::string> ConfigLoader::loadConfigPair(std::string input) {
    std::size_t i;

    std::string name, value;

    for (i = 0; i < input.size() && input[i] != '='; i++) {
        name += input[i];
    }

    for (i++; i < input.size(); i++) {
        value += input[i];
    }

    return std::make_tuple(name, value);
}

std::string ConfigLoader::getString(std::string name) {
    for (auto t: this->data) {
        if (std::get<0>(t).compare(name) == 0) {
            return std::get<1>(t);
        }
    }
    return "";
}

int ConfigLoader::getInt(std::string name) {
    std::string result = this->getString(name);

    try {
        return std::stof(result.c_str());
    } catch (std::exception *e) {
        return 0;
    }
}

float ConfigLoader::getFloat(std::string name) {
    std::string result = this->getString(name);

    try {
        return atof(result.c_str());
    } catch (std::exception *e) {
        return 0.0f;
    }
}

std::vector<std::string> ConfigLoader::getVariables() {
    std::vector<std::string> result;
    for (auto t: this->data) {
        result.push_back(std::get<0>(t));
    }
    return result;
}