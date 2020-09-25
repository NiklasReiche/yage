#include <catch2/catch.hpp>
#include <utils/strings.h>

TEST_CASE("Strip Test")
{
	SECTION("words separated by a blank")
	{
		auto words = utils::strip("This is a sentence.", " ");

		REQUIRE(words.size() == 4);
		CHECK(words[0] == "This");
		CHECK(words[1] == "is");
		CHECK(words[2] == "a");
		CHECK(words[3] == "sentence.");
	}
}

TEST_CASE("Replace Test")
{
	SECTION("replace single character")
	{
		std::string input = "Hello World!";

		CHECK("Hellxy Wxyrld!" == utils::replaceAll(input, "o", "xy"));
	}

	SECTION("replace multiple characters")
	{
		std::string input = "Hello World!";

		CHECK("Hexo World!" == utils::replaceAll(input, "ll", "x"));
	}
}
