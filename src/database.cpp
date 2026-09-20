#include <database.h>
#include <logger.h>

#include <stdexcept>
#include <fstream>
#include <sstream>

Database::Database(std::filesystem::path const& filepath)
	: m_filepath(filepath)
{
	LoadDatabase();
}

Database::~Database()
{
	StoreDatabase();
}

auto Database::LoadDatabase() -> void
{
	auto infile = std::ifstream(m_filepath);
	if (!infile)
	{
		LogFatal("failed to open file: '{}'", m_filepath.string());
	}

	auto line = std::string();
	while (std::getline(infile, line))
	{
		auto key = std::string();
		auto value = std::string();
		auto stream = std::istringstream(line);
		stream >> key >> value;
		if (key.empty() || value.empty())
		{
			LogFatal("failed to parse database file due to invalid line");
		}
		m_data.insert({ key, value });
	}
}

auto Database::StoreDatabase() -> void
{
	auto outfile = std::ofstream(m_filepath);
	if (!outfile)
	{
		LogFatal("failed to open file: '{}'", m_filepath.string());
	}

	for (auto const& [key, value] : m_data)
	{
		outfile << key << ' ' << value << '\n';
	}
}

auto Database::Insert(std::string const& key, std::string const& value) -> int
{
	if (m_data.contains(key))
	{
		return 0;
	}
	m_data.insert({ key, value });
	return 1;
}

auto Database::Select(std::string const& key) -> std::optional<std::string>
{
	if (!m_data.contains(key))
	{
		return std::nullopt;
	}
	return m_data.at(key);
}

auto Database::Update(std::string const& key, std::string const& value) -> int
{
	if (!m_data.contains(key))
	{
		return 0;
	}
	m_data.at(key) = value;
	return 1;
}

auto Database::Delete(std::string const& key) -> int
{
	if (!m_data.contains(key))
	{
		return 0;
	}
	m_data.erase(key);
	return 1;
}
