#include <gtest/gtest.h>

#include <logger.h>
#include <database.h>

#include <filesystem>
#include <fstream>

struct TempDir
{
private:

	std::filesystem::path m_root;

public:

	TempDir()
		: m_root{ std::filesystem::temp_directory_path() / "tmp" }
	{
		std::filesystem::create_directories(m_root);
	}

	~TempDir()
	{
		std::filesystem::remove_all(m_root);
	}

	auto Root() const -> std::filesystem::path
	{
		return m_root;
	}

};

auto MakeDatabaseFile(std::filesystem::path path, std::unordered_map<std::string, std::string> const& pairs) -> void
{
	auto outfile = std::ofstream(path);
	if (!outfile)
	{
		LogFatal("failed to open file: '{}'", path.string());
	}

	for (auto const& [ key, value ] : pairs)
	{
		outfile << key << ' ' << value << '\n';
	}
}

TEST(DatabaseTests, LoadDatabaseFromDisk)
{
	auto dir = TempDir();

	MakeDatabaseFile(
		(dir.Root() / "database.db"),
	{
		{ "key", "value" }
	});

	auto database = Database(dir.Root() / "database.db");

	{
		auto result = database.Select("key");
		ASSERT_TRUE(static_cast<bool>(result));
		EXPECT_EQ(*result, "value");
	}

	{
		auto result = database.Select("dne");
		ASSERT_FALSE(static_cast<bool>(result));
	}

	{
		auto result = database.Insert("key2", "value2");
		ASSERT_EQ(result, 1);
	}

	{
		auto result = database.Insert("key2", "value2");
		ASSERT_EQ(result, 0);
	}

	{
		auto result = database.Update("key2", "updated");
		ASSERT_EQ(result, 1);
	}

	{
		auto result = database.Update("key3", "updated");
		ASSERT_EQ(result, 0);
	}

	{
		auto result = database.Delete("key2");
		ASSERT_EQ(result, 1);
	}

	{
		auto result = database.Delete("key3");
		ASSERT_EQ(result, 0);
	}
}
