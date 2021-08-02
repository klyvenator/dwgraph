#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <memory>

TEST_CASE("CONSTRUCTOR - Initializer list") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};

	SECTION("Node correctly inserted") {
		CHECK(g.is_node("hello") == true);
		CHECK(g.is_node("goodbye") == true);
		CHECK(g.is_node("hi") == true);
	}
}

// Test with find after implemented
TEST_CASE("COPY CONSTRUCTOR") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("hello", "hi", 4);
	auto g2 = g;

	SECTION("Contains the same nodes") {
		CHECK(g2.is_node("hello") == true);
		CHECK(g2.is_node("goodbye") == true);
		CHECK(g2.is_node("hi") == true);
	}

	SECTION("Contains the same edges") {
		// To be implemented
	}
}

TEST_CASE("COPY ASSIGNMENT") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("hello", "hi", 4);
	auto g2 = gdwg::graph<std::string, int>{std::initializer_list<std::string>{"no", "lol"}};
	g2.insert_edge("no", "lol", 5);
	g2 = g;

	SECTION("Contains the same nodes") {
		CHECK(g2.is_node("hello") == true);
		CHECK(g2.is_node("goodbye") == true);
		CHECK(g2.is_node("hi") == true);
	}

	SECTION("Contains the same edges") {
		// To be implemented
	}
}

TEST_CASE("MOVE CONSTRUCTOR") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("hello", "hi", 4);
	auto const g2 = gdwg::graph<std::string, int>{std::move(g)};

	SECTION("Node correctly inserted") {
		CHECK(g2.is_node("hello") == true);
		CHECK(g2.is_node("goodbye") == true);
		CHECK(g2.is_node("hi") == true);
	}
}

TEST_CASE("MOVE ASSIGNMENT") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	auto g2 = gdwg::graph<std::string, int>{};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("hello", "hi", 4);
	g2 = gdwg::graph<std::string, int>{std::move(g)};
	SECTION("Node correctly inserted") {
		CHECK(g2.is_node("hello") == true);
		CHECK(g2.is_node("goodbye") == true);
		CHECK(g2.is_node("hi") == true);
	}
}

TEST_CASE("INSERT NODE") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	SECTION("Node correctly inserted") {
		CHECK(g.insert_node("lol") == true);
		CHECK(g.is_node("lol") == true);
	}
}

TEST_CASE("INSERT EDGE") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	SECTION("Edge correctly inserted") {
		CHECK(g.insert_edge("hello", "goodbye", 2) == true);
		CHECK(g.insert_edge("hello", "goodbye", 3) == true);
	}
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("hi", "goodbye", 2);

	SECTION("Edge fails if it exists already") {
		CHECK(g.insert_edge("hello", "goodbye", 3) == false);
	}

	SECTION("Edge exception if either node does not exist") {
		CHECK_THROWS_WITH(g.insert_edge("lol", "hello", 3),
		                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                  "the graph");
		CHECK_THROWS_WITH(g.insert_edge("hello", "k", 4),
		                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                  "the graph");
	}
}

TEST_CASE("REPLACE NODE") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("hello", "hi", 4);

	SECTION("EXCEPTION - Old node doesn't exist") {
		CHECK_THROWS_WITH(g.replace_node("lol", "hello"),
		                  "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
	}

	SECTION("New node already exists") {
		CHECK(g.replace_node("hello", "hi") == false);
	}

	SECTION("Successfully replaces node") {
		CHECK(g.replace_node("hello", "lol") == true);
		CHECK(g.is_node("lol") == true);
		CHECK(g.is_node("hello") == false);
		// EDGE CHECK WITH FIND EDGE
	}
}

TEST_CASE("MERGE REPLACE NODE") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("goodbye", "hi", 8);
	g.insert_edge("hello", "hi", 4);
	g.insert_edge("goodbye", "goodbye", 5);

	SECTION("EXCEPTION - Old node or new node doesn't exist") {
		CHECK_THROWS_WITH(g.merge_replace_node("lol", "k"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                  "they don't exist in the graph");
		CHECK_THROWS_WITH(g.merge_replace_node("lol", "hello"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                  "they don't exist in the graph");
		CHECK_THROWS_WITH(g.merge_replace_node("hello", "k"),
		                  "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if "
		                  "they don't exist in the graph");
	}
	SECTION("General merging test") {
		g.merge_replace_node("goodbye", "hello");
		// auto it = g.begin();
		// CHECK((*it).from == "hello");
	}
}

TEST_CASE("ITERATOR CONSTRUCTORS") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("hello", "hi", 4);

	SECTION("iterator for non-const graph") {
		auto it = g.begin();
		CHECK((*it).from == "hello");
		CHECK((*it).to == "goodbye");
		CHECK((*it).weight == 2);
	}

	auto const g3 = g;

	SECTION("iterator for const graph") {
		auto const it = g3.begin();
		CHECK((*it).from == "hello");
		CHECK((*it).to == "goodbye");
		CHECK((*it).weight == 2);
	}

	SECTION("Value construction iterator") {
		auto const it = gdwg::graph<int, int>::iterator{};
		auto const it2 = gdwg::graph<int, int>::iterator{};
		CHECK(it == it2);
	}
}

TEST_CASE("ITERATOR OPERATOR ++") {
	auto list = std::initializer_list<std::string>{"a", "b", "c"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("a", "b", 2);
	g.insert_edge("a", "a", 3);
	g.insert_edge("a", "a", 1);

	SECTION("Pre-increment") {
		auto it = g.begin();
		CHECK((*it).weight == 1);
		CHECK((*++it).weight == 3);
		CHECK((*++it).weight == 2);
	}

	SECTION("Post-increment") {
		auto it = g.begin();
		CHECK((*it).weight == 1);
		CHECK((*it++).weight == 1);
		CHECK((*it++).weight == 3);
	}
}
