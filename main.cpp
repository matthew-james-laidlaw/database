#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

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
}

auto main() -> int
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
