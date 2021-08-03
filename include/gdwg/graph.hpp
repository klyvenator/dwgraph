#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <unordered_set>
#include <vector>

namespace gdwg {

	template<typename N, typename E>
	class graph {
	public:
		using adj_tuple = std::tuple<std::shared_ptr<N>, std::shared_ptr<N>, std::shared_ptr<E>>;
		using adj_list_container = std::vector<adj_tuple>;
		using node_list_container = std::vector<std::shared_ptr<N>>;
		struct value_type {
			value_type(N first, N second, E third)
			: from{first}
			, to{second}
			, weight{third} {};

			N from;
			N to;
			E weight;
		};

		class iterator {
		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using adj_iterator = typename adj_list_container::const_iterator;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			friend class graph;

			auto operator*() const -> reference {
				return reference{*std::get<0>(*it_), *std::get<1>(*it_), *std::get<2>(*it_)};
			}
			auto operator++() -> iterator& {
				++it_;
				return *this;
			}
			auto operator++(int) -> iterator {
				auto copy = *this;
				++*this;
				return copy;
			}
			auto operator--() -> iterator& {
				--it_;
				return *this;
			}
			auto operator--(int) -> iterator {
				auto copy = *this;
				--*this;
				return copy;
			}

			auto operator==(iterator const& other) const -> bool;

		private:
			iterator() = default;
			explicit iterator(const adj_list_container& adj, adj_iterator it)
			: pointee_{&adj}
			, it_{it} {};
			adj_list_container const* pointee_ = nullptr;
			adj_iterator it_{};
		};

		// Constructors
		graph() = default;
		graph(std::initializer_list<N> il);
		template<typename InputIt>
		graph(InputIt first, InputIt last);
		graph(graph&& other) noexcept
		: node_list_{std::exchange(other.node_list_, node_list_container{})}
		, adj_list_{std::exchange(other.adj_list_, adj_list_container{})} {}
		graph(graph const& other)
		: node_list_{other.node_list_}
		, adj_list_{other.adj_list_} {}
		~graph() = default;

		// Operators
		auto operator=(graph&& other) noexcept -> graph&;
		auto operator=(graph const& other) -> graph&;
		[[nodiscard]] auto operator==(graph const& other) const -> bool;
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			if (g.node_list_.empty()) {
				return os;
			}

			auto current_node = g.node_list_.begin();
			auto current_edge = g.begin();
			while (current_node != g.node_list_.end()) {
				os << **current_node << " (\n";
				while (current_edge != g.end() && (*current_edge).from == **current_node) {
					os << "  " << (*current_edge).to << " | " << (*current_edge).weight << "\n";
					current_edge++;
				}
				os << ")\n";
				current_node++;
			}
			return os;
		}

		// Modifiers
		auto insert_node(N const& value) -> bool;
		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool;
		auto insert_edge(value_type tup) -> bool {
			return insert_edge(tup.from, tup.to, tup.weight);
		}
		auto replace_node(N const& old_data, N const& new_data) -> bool;
		auto merge_replace_node(N const& old_data, N const& new_data) -> void;
		auto erase_node(N const& value) -> bool;
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool;
		auto erase_edge(iterator i) -> iterator {
			return iterator{adj_list_, adj_list_.erase(i.it_)};
		}
		auto erase_edge(iterator i, iterator s) -> iterator {
			return iterator{adj_list_, adj_list_.erase(i.it_, s.it_)};
		};
		auto clear() noexcept -> void {
			adj_list_.clear();
			node_list_.clear();
		}

		// Accessors
		[[nodiscard]] auto is_node(N const& value) const -> bool;
		[[nodiscard]] auto empty() -> bool {
			return node_list_.empty();
		}
		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool;
		[[nodiscard]] auto nodes() -> std::vector<N>;
		[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E>;
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator;
		[[nodiscard]] auto connections(N const& src) -> std::vector<N>;

		// Iterator access
		[[nodiscard]] auto begin() const -> iterator {
			return iterator{adj_list_, adj_list_.begin()};
		}
		[[nodiscard]] auto end() const -> iterator {
			return iterator{adj_list_, adj_list_.end()};
		}

		// OTHER
		auto static tuple_sort(adj_tuple a, adj_tuple b) -> bool;

	private:
		node_list_container node_list_{};
		adj_list_container adj_list_{};
		// Finds pointer to node heap resource and returns it
		auto get_node(N const& value) -> std::shared_ptr<N>;
		// Finds pointer to an edge weight resource and returns it (Used to prevent making duplicate
		// edges)
		auto get_edge(E const& value) -> std::shared_ptr<E>;
		// Determines if edge weight already exists as heap resource
		[[nodiscard]] auto is_edge(E const& value) const -> bool;
	};

	template<typename N, typename E>
	template<typename InputIt>
	graph<N, E>::graph(InputIt first, InputIt last) {
		while (first != last) {
			insert_node(*first);
			first++;
		}
	}

	template<typename N, typename E>
	auto graph<N, E>::iterator::operator==(iterator const& other) const -> bool {
		if (pointee_ != other.pointee_) {
			return false;
		}
		if (pointee_ == nullptr && other.pointee_ == nullptr) {
			return true;
		}
		if (pointee_ == nullptr || other.pointee_ == nullptr) {
			return false;
		}
		if (it_ == pointee_->end() && other.it_ == pointee_->end()) {
			return true;
		}
		if (it_ == pointee_->end() || other.it_ == pointee_->end()) {
			return false;
		}
		return !static_cast<bool>((**this).from != (*other).from || (**this).to != (*other).to
		                          || (**this).weight != (*other).weight);
	}

	template<typename N, typename E>
	graph<N, E>::graph(std::initializer_list<N> il) {
		for (auto i = il.begin(); i != il.end(); i++) {
			insert_node(*i);
		}
	}

	template<typename N, typename E>
	auto graph<N, E>::operator=(graph&& other) noexcept -> graph& {
		std::swap(node_list_, other.node_list_);
		std::swap(adj_list_, other.adj_list_);
		other.node_list_ = node_list_container{};
		other.adj_list_ = adj_list_container{};
		return *this;
	}

	template<typename N, typename E>
	// NOLINTNEXTLINE
	auto graph<N, E>::operator=(graph const& other) -> graph& {
		auto new_node_list = other.node_list_;
		auto new_adj_list = other.adj_list_;
		node_list_ = new_node_list;
		adj_list_ = new_adj_list;
		return *this;
	}

	template<typename N, typename E>
	auto graph<N, E>::insert_node(N const& value) -> bool {
		for (auto node : node_list_) {
			if (*node == value) {
				return false;
			}
		}
		node_list_.push_back(std::make_shared<N>(value));
		std::sort(node_list_.begin(),
		          node_list_.end(),
		          [](std::shared_ptr<N> a, std::shared_ptr<N> b) -> bool { return (*a < *b); });
		return true;
	}

	template<typename N, typename E>
	auto graph<N, E>::tuple_sort(adj_tuple a, adj_tuple b) -> bool {
		if (*std::get<0>(a) < *std::get<0>(b)) {
			return true;
		}
		if (*std::get<0>(a) == *std::get<0>(b)) {
			if (*std::get<1>(a) < *std::get<1>(b)) {
				return true;
			}
			if (*std::get<1>(a) == *std::get<1>(b)) {
				if (*std::get<2>(a) < *std::get<2>(b)) {
					return true;
				}
			}
		}
		return false;
	}

	template<typename N, typename E>
	auto graph<N, E>::insert_edge(N const& src, N const& dst, E const& weight) -> bool {
		if (!is_node(src) || !is_node(dst)) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node don't "
			                         "exist in the graph");
			return false;
		}

		if (find(src, dst, weight) != end()) {
			return false;
		}

		std::shared_ptr<E> edge;
		if (is_edge(weight)) {
			edge = get_edge(weight);
		}
		else {
			edge = std::make_shared<E>(weight);
		}
		auto tuple_edge = adj_tuple{get_node(src), get_node(dst), edge};
		adj_list_.push_back(tuple_edge);
		std::sort(adj_list_.begin(), adj_list_.end(), tuple_sort);
		return true;
	}

	template<typename N, typename E>
	auto graph<N, E>::replace_node(N const& old_data, N const& new_data) -> bool {
		if (is_node(old_data) == false) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
			                         "doesn't exist");
		}

		if (is_node(new_data)) {
			return false;
		}

		insert_node(new_data);
		auto const new_node = get_node(new_data);
		for (auto& tuple : adj_list_) {
			if (*std::get<0>(tuple) == old_data) {
				std::get<0>(tuple) = new_node;
			}
			if (*std::get<1>(tuple) == old_data) {
				std::get<1>(tuple) = new_node;
			}
		}

		for (auto it = node_list_.begin(); it != node_list_.end(); ++it) {
			if (**it == old_data) {
				node_list_.erase(it);
				break;
			}
		}
		return true;
	}

	template<typename N, typename E>
	auto graph<N, E>::merge_replace_node(N const& old_data, N const& new_data) -> void {
		if (is_node(old_data) == false || is_node(new_data) == false) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new "
			                         "data if they don't exist in the graph");
		}
		auto vec = std::vector<value_type>{};
		for (auto tuple : adj_list_) {
			if (*std::get<0>(tuple) == old_data && *std::get<1>(tuple) == old_data) {
				vec.push_back(value_type{new_data, new_data, *std::get<2>(tuple)});
			}
			else if (*std::get<0>(tuple) == old_data) {
				vec.push_back(value_type{new_data, *std::get<1>(tuple), *std::get<2>(tuple)});
			}
			else if (*std::get<1>(tuple) == old_data) {
				vec.push_back(value_type{*std::get<0>(tuple), new_data, *std::get<2>(tuple)});
			}
		}

		for (auto i : vec) {
			insert_edge(i);
		}

		erase_node(old_data);
	}

	template<typename N, typename E>
	auto graph<N, E>::erase_edge(N const& src, N const& dst, E const& weight) -> bool {
		if (is_node(src) == false || is_node(dst) == false) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they "
			                         "don't exist in the graph");
		}
		auto it = find(src, dst, weight);
		if (it != end()) {
			erase_edge(it);
			return true;
		}
		return false;
	}

	template<typename N, typename E>
	auto graph<N, E>::erase_node(N const& value) -> bool {
		if (!is_node(value)) {
			return false;
		}
		auto it = adj_list_.begin();
		while (it != adj_list_.end()) {
			if (*std::get<0>(*it) == value || *std::get<1>(*it) == value) {
				it = adj_list_.erase(it);
			}
			else {
				++it;
			}
		}

		for (auto it2 = node_list_.begin(); it2 != node_list_.end(); ++it2) {
			if (**it2 == value) {
				node_list_.erase(it2);
				break;
			}
		}
		return true;
	}

	template<typename N, typename E>
	auto graph<N, E>::get_node(N const& value) -> std::shared_ptr<N> {
		for (auto node : node_list_) {
			if (*node == value) {
				return node;
			}
		}
		return nullptr;
	}

	template<typename N, typename E>
	auto graph<N, E>::get_edge(E const& value) -> std::shared_ptr<E> {
		for (auto tuple : adj_list_) {
			auto edge = std::get<2>(tuple);
			if (*edge == value) {
				return edge;
			}
		}
		return nullptr;
	}

	template<typename N, typename E>
	auto graph<N, E>::is_edge(E const& value) const -> bool {
		for (auto tuple : adj_list_) {
			auto edge = std::get<2>(tuple);
			if (*edge == value) {
				return true;
			}
		}
		return false;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N, E>::is_node(N const& value) const -> bool {
		// NOLINTNEXTLINE
		for (auto node : node_list_) {
			if (*node == value) {
				return true;
			}
		}
		return false;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N, E>::is_connected(N const& src, N const& dst) -> bool {
		if (is_node(src) == false || is_node(dst) == false) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node "
			                         "don't exist in the graph");
		}
		for (auto it = begin(); it != end(); ++it) {
			if ((*it).from == src && (*it).to == dst) {
				return true;
			}
		}
		return false;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N, E>::nodes() -> std::vector<N> {
		auto vec = std::vector<N>{};
		for (auto node : node_list_) {
			vec.push_back(*node);
		}
		return vec;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N, E>::weights(N const& src, N const& dst) -> std::vector<E> {
		if (is_node(src) == false || is_node(dst) == false) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node don't "
			                         "exist "
			                         "in the graph");
		}

		auto vec = std::vector<E>{};
		for (auto it = begin(); it != end(); ++it) {
			if ((*it).from == src && (*it).to == dst) {
				vec.push_back((*it).weight);
			}
		}
		return vec;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N, E>::connections(N const& src) -> std::vector<N> {
		if (is_node(src) == false) {
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist "
			                         "in the graph");
		}
		auto s = std::set<N>{};
		for (auto it = begin(); it != end(); ++it) {
			if ((*it).from == src) {
				s.insert((*it).to);
			}
		}
		auto vec = std::vector<N>{};
		vec.assign(s.begin(), s.end());
		return vec;
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N, E>::find(N const& src, N const& dst, E const& weight) -> iterator {
		for (auto i = begin(); i != end(); ++i) {
			if ((*i).from == src && (*i).to == dst && (*i).weight == weight) {
				return i;
			}
		}
		return end();
	}

	template<typename N, typename E>
	[[nodiscard]] auto graph<N, E>::operator==(graph const& other) const -> bool {
		if (node_list_.size() != other.node_list_.size() || adj_list_.size() != other.adj_list_.size())
		{
			return false;
		}
		for (auto i = node_list_.begin(), j = other.node_list_.begin(); i != node_list_.end(); ++i, ++j)
		{
			if (**i != **j) {
				return false;
			}
		}

		for (auto i = adj_list_.begin(), j = other.adj_list_.begin(); i != adj_list_.end(); ++i, ++j) {
			if (*std::get<0>(*i) != *std::get<0>(*j) || *std::get<1>(*i) != *std::get<1>(*j)
			    || *std::get<2>(*i) != *std::get<2>(*j))
			{
				return false;
			}
		}
		return true;
	}

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
