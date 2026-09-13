#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>

namespace fs = std::filesystem;

auto LogInfo(std::string const& msg) -> void
{
    std::cout << std::format("[Info] {}\n", msg);
}

class Database
{
private:

    std::unordered_map<std::string, std::string> m_data;

public:

    Database(fs::path const& filepath)
    {
        ParseDatabaseFile(filepath);
    }

private:

    auto ParseDatabaseFile(fs::path const& filepath) -> void
    {
        auto infile = std::ifstream(filepath);
        if (!infile)
        {
            throw std::runtime_error(std::format("[Error] failed to open file: '{}'", filepath.string()));
        }

        auto line = std::string();
        while (std::getline(infile, line))
        {
            const auto& [key, value] = ParseLine(line);
            
            if (m_data.contains(key))
            {
                throw std::runtime_error(std::format("[Error] could not load database due to duplicate key: '{}'", key));
            }

            LogInfo(std::format("loaded key-value pair: '{}', '{}'", key, value));
            m_data.insert({key, value});
        }
    }

    auto ParseLine(std::string const& line) -> std::pair<std::string, std::string>
    {
        auto delim = line.find_first_of(' ');
        auto key = line.substr(0, delim);
        auto value = line.substr(delim + 1);
        return { key, value };
    }

};

auto App() -> void
{
    auto cwd = fs::current_path();
    auto dbfile = cwd / "database.db";

    if (!fs::exists(dbfile))
    {
        throw std::runtime_error(std::format("[Error] Could not find database.db in current directory: '{}'\n", dbfile.string()));
    }

    auto db = Database(dbfile);

    bool done = false;
    while (!done)
    {
        std::cout << "> ";

        std::string line;
        std::getline(std::cin, line, '\n');

        std::istringstream stream(line);

        std::string command;
        stream >> command;

        if (command == "create")
        {
            std::string key;
            std::string value;
            stream >> key >> value;
            std::cout << std::format("command: '{}', key: '{}', value: '{}'\n", command, key, value);
        }
        else if (command == "read")
        {
            std::string key;
            stream >> key;
            std::cout << std::format("command: '{}', key: '{}'\n", command, key);
        }
        else if (command == "update")
        {
            std::string key;
            std::string new_value;
            stream >> key >> new_value;
            std::cout << std::format("command: '{}', key: '{}', value: '{}'\n", command, key, new_value);
        }
        else if (command == "delete")
        {
            std::string key;
            stream >> key;
            std::cout << std::format("command: '{}', key: '{}'\n", command, key);
        }
        else if (command == "exit")
        {
            done = true;
        }
        else
        {
            std::cout << std::format("unrecognized command: '{}'\n", command);
        }
    }
}

auto main(int argc, char** argv) -> int
{
    try
    {
        App();
    }
    catch (std::exception const& err)
    {
        std::cerr << err.what() << '\n';
        return 1;
    }

    return 0;
}
