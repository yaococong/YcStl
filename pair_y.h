template <class T1, class T2>
struct pair {
	friend void swap(pair& p1, pair& p2);
	using first_type = T1;
	using second_type = T2;

	T1 first;
	T2 second;

	consexpr pair() = default;

	template <class U, class V>
	pair(const pair<U, V>& pr) : first(pr.first), second(pr.second) {}
	template <class U, class V>
	pair(pair<U, V>&& pr) : first(std::move(pr.first)), second(std::move(pr.second)) {}
	pair(const pair& pr) = default;
	pair(pair&& pr) = default;

	pair(const first_type& a, const first_type& b) : first(a), second(b) {}
	template<class U, class V>
	pair(U&& a, V&& b);

	pair& operator=(const pair& pr) {
		first = pr.first;
		second = pr.second;
		return *this;
	}
	template <class U, class V>
	pair& operator=(const pair<U, V>& pr) {
		first = pr.first;
		second = pr.second;
		return *this;
	}
	pair& operator=(pair&& pr) {
		first = std::move(pr.first);
		second = std::move(pr.second);
		return *this;
	}

	void swap(pair& pr) {
		swap(*this, pr);
	}

};

template <class T1, class T2>
void swap(pair<T1, T2> &p1, pair<T1, T2> &p2) {
	using std::swap;
	swap(p1.first, p2.first);
	swap(p1.second, p2.second);
}