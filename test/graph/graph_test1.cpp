#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <initializer_list>
#include <memory>
#include <sstream>

TEST_CASE("CONSTRUCTOR - No args") {
	SECTION("Can be instantiated and is empty") {
		auto g = gdwg::graph<std::string, int>{};
		CHECK(g.empty() == true);
	}
	SECTION("Can insert nodes & edges") {
		auto g = gdwg::graph<std::string, int>{};
		CHECK(g.insert_node("what"));
		CHECK(g.insert_node("hi"));
		CHECK(g.is_node("what"));
		CHECK(g.insert_edge("what", "hi", 3));
		CHECK(g.find("what", "hi", 3) != g.end());
	}
}

TEST_CASE("CONSTRUCTOR - Initializer list") {
	SECTION("Node correctly inserted") {
		auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
		auto g = gdwg::graph<std::string, int>{list};
		CHECK(g.is_node("hello") == true);
		CHECK(g.is_node("goodbye") == true);
		CHECK(g.is_node("hi") == true);
	}

	SECTION("Empty initializer") {
		auto g = gdwg::graph<std::string, int>{std::initializer_list<std::string>{}};
		CHECK(g.empty() == true);
	}
}

TEST_CASE("CONSTRUCTOR - InputIt") {
	SECTION("Node correctly inserted") {
		auto list = std::vector<std::string>{"hello", "goodbye", "hi"};
		auto g = gdwg::graph<std::string, int>{list.begin(), list.end()};
		CHECK(g.is_node("hello") == true);
		CHECK(g.is_node("goodbye") == true);
		CHECK(g.is_node("hi") == true);
	}

	SECTION("Empty initializer") {
		auto list = std::vector<std::string>{};
		auto g = gdwg::graph<std::string, int>{list.begin(), list.end()};
		CHECK(g.empty() == true);
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

TEST_CASE("OPERATOR ==") {
	SECTION("Node Comparison") {
		auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
		auto g = gdwg::graph<std::string, int>{list};
		auto list2 = std::initializer_list<std::string>{"hello"};
		auto g2 = gdwg::graph<std::string, int>{list2};
		CHECK((g == g2) == false);
		g2.insert_node("goodbye");
		g2.insert_node("hi");
		CHECK((g == g2) == true);
		g.insert_node("lol");
		g2.insert_node("lol2");
		CHECK((g == g2) == false);
	}

	SECTION("Edge Comparison") {
		auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
		auto g = gdwg::graph<std::string, int>{list};

		g.insert_edge("hello", "goodbye", 2);
		g.insert_edge("hello", "goodbye", 3);
		g.insert_edge("goodbye", "hi", 8);
		g.insert_edge("hello", "hi", 4);
		g.insert_edge("goodbye", "goodbye", 5);

		auto g2 = g;

		CHECK((g == g2) == true);
		g.erase_edge("hello", "goodbye", 2);
		CHECK((g == g2) == false);
	}

	SECTION("Empty comparison") {
		auto const g = gdwg::graph<std::string, int>{};
		auto const g2 = gdwg::graph<std::string, int>{};
		CHECK((g == g2) == true);
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
		auto it = g.begin();
		CHECK(((*it).from == "hello" && (*it).to == "hello" && (*it).weight == 2));
		it++;
		CHECK(((*it).from == "hello" && (*it).to == "hello" && (*it).weight == 3));
		it++;
		CHECK(((*it).from == "hello" && (*it).to == "hello" && (*it).weight == 5));
		it++;
		CHECK(((*it).from == "hello" && (*it).to == "hi" && (*it).weight == 4));
		it++;
		CHECK(((*it).from == "hello" && (*it).to == "hi" && (*it).weight == 8));
		CHECK(g.is_node("goodbye") == false);
	}

	SECTION("Duplicates removed during merge") {
		g.insert_edge("hello", "hello", 2);
		g.merge_replace_node("goodbye", "hello");
		auto it = g.begin();
		CHECK(((*it).from == "hello" && (*it).to == "hello" && (*it).weight == 2));
		it++;
		CHECK(((*it).from == "hello" && (*it).to == "hello" && (*it).weight == 3));
	}
}

TEST_CASE("ERASE NODE") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("goodbye", "hello", 8);
	g.insert_edge("hello", "hi", 4);
	g.insert_edge("goodbye", "goodbye", 5);
	g.erase_node("hello");
	SECTION("Check node has been removed") {
		CHECK(g.is_node("hello") == false);
	}
	SECTION("Check edges have been removed") {
		auto it = g.begin();
		CHECK((*it).weight == 5);
		CHECK((*it).from == "goodbye");
		CHECK((*it).to == "goodbye");
		it++;
		CHECK(it == g.end());
	}
	SECTION("No node removed because it doesn't exist") {
		CHECK(g.erase_node("lol") == false);
	}
}

TEST_CASE("ERASE EDGE") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("goodbye", "hello", 8);
	g.insert_edge("hello", "hi", 4);
	g.insert_edge("goodbye", "goodbye", 5);

	SECTION("erase_edge(N, N, E)") {
		SECTION("either src or dest node doesnt exist exception") {
			CHECK_THROWS_WITH(g.erase_edge("lol", "hi", 4),
			                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
			                  "exist in the graph");
			CHECK_THROWS_WITH(g.erase_edge("hello", "hey", 2),
			                  "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't "
			                  "exist in the graph");
		}
		SECTION("edge doesnt exist") {
			CHECK(g.erase_edge("hello", "goodbye", 5) == false);
			CHECK(g.erase_edge("hello", "hi", 2) == false);
		}
		SECTION("edge successfully erased") {
			CHECK(g.erase_edge("hello", "goodbye", 3) == true);
			CHECK(g.find("hello", "goodbye", 3) == g.end());
		}
	}

	SECTION("erase_edge(iterator)") {
		auto it = g.begin();
		it++;
		it++;
		g.erase_edge(it);
		CHECK(g.find("hello", "goodbye", 2) == g.end());
	}

	SECTION("erase_edge(iterator)") {
		auto it = g.begin();
		it++;
		auto it2 = it;
		it2++;
		it2++;
		it2++;
		g.erase_edge(it, it2);
		it = g.begin();
		CHECK(((*it).from == "goodbye" && (*it).to == "goodbye" && (*it).weight == 5));
		++it;
		CHECK(((*it).from == "hello" && (*it).to == "hi" && (*it).weight == 4));
	}
}

TEST_CASE("CLEAR") {
	SECTION("Filled graph - Check nodes & edges removed") {
		auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
		auto g = gdwg::graph<std::string, int>{list};
		g.insert_edge("hello", "goodbye", 2);
		g.insert_edge("hello", "goodbye", 3);
		g.insert_edge("goodbye", "hello", 8);
		g.insert_edge("hello", "hi", 4);
		g.insert_edge("goodbye", "goodbye", 5);
		g.erase_node("hello");
		g.clear();
		CHECK(g.empty() == true);
		auto it = g.begin();
		CHECK(it == g.end());
	}
	SECTION("Empty graph") {
		auto g = gdwg::graph<std::string, int>{};
		CHECK(g.empty() == true);
		auto it = g.begin();
		CHECK(it == g.end());
	}
}

TEST_CASE("Is_Connected") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("goodbye", "hello", 8);
	g.insert_edge("hello", "hi", 4);
	g.insert_edge("goodbye", "goodbye", 5);

	SECTION("Exception thrown if one of the nodes missing") {
		CHECK_THROWS_WITH(g.is_connected("a", "hi"),
		                  "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                  "exist in the graph");
		CHECK_THROWS_WITH(g.is_connected("hello", "b"),
		                  "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't "
		                  "exist in the graph");
	}

	SECTION("Edge exists") {
		CHECK(g.is_connected("hello", "hi") == true);
	}

	SECTION("Edge doesn't exist") {
		CHECK(g.is_connected("goodbye", "hi") == false);
		CHECK(g.is_connected("hi", "hello") == false);
	}
}

TEST_CASE("Node list") {
	auto list = std::initializer_list<int>{4, 2, 3, 7};
	auto g = gdwg::graph<int, std::string>{list};
	auto v = std::vector{2, 3, 4, 7};
	CHECK(g.nodes() == v);
}

TEST_CASE("Weights") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("goodbye", "hello", 8);
	g.insert_edge("hello", "hi", 4);
	g.insert_edge("goodbye", "goodbye", 5);

	SECTION("Multiple weights") {
		auto v = std::vector<int>{2, 3};
		CHECK(g.weights("hello", "goodbye") == v);
	}
	SECTION("No weights") {
		auto v = std::vector<int>{};
		CHECK(g.weights("hello", "hello") == v);
	}
	SECTION("Connects to self") {
		auto v = std::vector<int>{5};
		CHECK(g.weights("goodbye", "goodbye") == v);
	}

	SECTION("Exception throw if src or dst doesn't exist") {
		CHECK_THROWS_WITH(g.weights("hello", "b"),
		                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                  "the graph");
		CHECK_THROWS_WITH(g.weights("a", "goodbye"),
		                  "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in "
		                  "the graph");
	}
}

TEST_CASE("Find") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("goodbye", "hello", 8);
	g.insert_edge("hello", "hi", 4);
	g.insert_edge("goodbye", "goodbye", 5);

	SECTION("Edge exists") {
		auto it = g.find("hello", "goodbye", 2);
		CHECK(((*it).from == "hello" && (*it).to == "goodbye" && (*it).weight == 2));
	}
	SECTION("Edge doesn't exist") {
		auto it = g.find("hello", "goodbye", 5);
		CHECK(it == g.end());
		it = g.find("hello", "hi", 2);
		CHECK(it == g.end());
		it = g.find("goodbye", "hi", 5);
		CHECK(it == g.end());
		it = g.find("lol", "ok", 5);
		CHECK(it == g.end());
	}
}

TEST_CASE("Connections") {
	auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
	auto g = gdwg::graph<std::string, int>{list};
	g.insert_edge("hello", "goodbye", 2);
	g.insert_edge("hello", "goodbye", 3);
	g.insert_edge("goodbye", "hello", 8);
	g.insert_edge("hello", "hi", 4);
	g.insert_edge("goodbye", "goodbye", 5);

	SECTION("General check - node has multiple connections") {
		auto v = std::vector<std::string>{"goodbye", "hi"};
		CHECK(g.connections("hello") == v);
	}
	SECTION("Node connected to self") {
		auto v = std::vector<std::string>{"goodbye", "hello"};
		CHECK(g.connections("goodbye") == v);
	}
	SECTION("Node has no connections") {
		g.insert_node("lol");
		auto v = std::vector<std::string>{};
		CHECK(g.connections("lol") == v);
	}
}

TEST_CASE("Iterator accessors - begin, end") {
	SECTION("Valid graph and begin iterator") {
		auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
		auto g = gdwg::graph<std::string, int>{list};
		g.insert_edge("hello", "goodbye", 2);
		g.insert_edge("hello", "goodbye", 3);
		g.insert_edge("goodbye", "hello", 8);
		g.insert_edge("hello", "hi", 4);
		g.insert_edge("goodbye", "goodbye", 5);

		auto it = g.begin();
		CHECK(((*it).from == "goodbye" && (*it).to == "goodbye" && (*it).weight == 5));
	}
	SECTION("Empty graph - begin should == end") {
		auto list = std::initializer_list<std::string>{"hello", "goodbye", "hi"};
		auto g = gdwg::graph<std::string, int>{list};
		auto it = g.begin();
		CHECK(it == g.end());
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

TEST_CASE("EXTRACTOR") {
	auto list = std::initializer_list<std::string>{"a", "b", "c"};
	auto g = gdwg::graph<std::string, int>{list};

	SECTION("No edge print") {
		auto out = std::ostringstream{};
		out << g;
		auto const expected_output = std::string_view(R"(a (
)
b (
)
c (
)
)");
		CHECK(out.str() == expected_output);
	}

	SECTION("Edge and node print") {
		auto out = std::ostringstream{};
		g.insert_edge("b", "b", 4);
		g.insert_edge("b", "b", 1);
		g.insert_edge("a", "c", 5);
		g.insert_node("e");
		g.insert_node("d");
		g.insert_edge("e", "c", 4);
		out << g;
		auto const expected_output = std::string_view(R"(a (
  c | 5
)
b (
  b | 1
  b | 4
)
c (
)
d (
)
e (
  c | 4
)
)");

		CHECK(out.str() == expected_output);
	}

	SECTION("Empty graph print") {
		auto out = std::ostringstream{};
		auto g2 = gdwg::graph<std::string, int>{};
		out << g2;
		auto const expected_output = std::string_view(R"()");
		CHECK(out.str() == expected_output);
	}
}
