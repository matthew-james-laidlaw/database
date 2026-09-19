#pragma once

#include <token.h>

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>

class Database
{
private:

	std::filesystem::path m_filepath;
	std::unordered_map<std::string, std::string> m_data;

public:

	Database(std::filesystem::path const& filepath);
	~Database();

	auto LoadDatabase() -> void;
	auto StoreDatabase() -> void;

	auto Insert(std::string const& key, std::string const& value) -> int;
	auto Select(std::string const& key) -> std::optional<std::string>;
	auto Update(std::string const& key, std::string const& value) -> int;
	auto Delete(std::string const& key) -> int;

};
