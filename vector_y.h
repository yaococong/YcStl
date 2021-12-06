#include "allocate_y.h"
#include "pair_y.h"
#include <initializer_list>

namespace yc {
	using namespace std;
	template <class T, class Alloc = allocator_y<T>>
	class vector_y {
	public:
		using value_type = T;
		using allocator_type = Alloc;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using cosnt_pointer = const T*;
		using iterator = T*;
		using const_iterator = const T*;
		// using reverse_iterator = reverse_iterator<iterator>;
		// using const_reverse_iterator = reverse_iterator<const_iterator>;
		using difference_type = iterator_traits<iterator>::difference_type;
		using size_type = size_t;

	public:
		vector_y() noexcept = default;
		explicit vector_y(const allocator_type& alloc) : data_allocator(alloc) {}

		explicit vector_y(size_type n, const allocator_type& alloc = allocator_type()) : data_allocator(alloc) {
			fill_initialize(n, T());
		}
		vector_y(size_type n, const value_type& val, const allocator_type& alloc = allocator_type()) : data_allocator(alloc) {
			fill_initialize(n, val);
		}

		template <class InputIterator>
		vector_y(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : data_allocator(alloc) {
			auto n = distance(first, last);
			start = data_allocator.allocate(n);
			uninitialized_copy_y(first, last, start);
			finish = start + n;
			end_of_storage = finish;
		}

		vector_y(const vector_y& x) : vector_y(x.begin(), x.end(), x.data_allocator) {

		}
		vector_y(const vector_y& x, const allocator_type& alloc) : vector_y(x.begin(), x.end()) {
			data_allocator = x.data_allocator;
		}

		vector_y(vector_y &&x) {
			start = x.start;
			finish = x.finish;
			end_of_storage = x.end_of_storage;
			data_allocator = x.data_allocator;

			x.start = x.finish = x.end_of_storage = nullptr;
		}
		vector_y(vector_y &&x, const allocator_type& alloc) {
			start = x.start;
			finish = x.finish;
			end_of_storage = x.end_of_storage;
			data_allocator = alloc;

			x.start = x.finish = x.end_of_storage = nullptr;
		}

		vector_y(initializer_list<value_type> il, const allocator_type& alloc = allocator_type()) : vector_y(il.begin(), il.end(), alloc) {}

		~vector_y() {}

		vector_y& operator=(const vector_y& x) {}
		vector_y& operator=(vector_y&& x) {}
		vector_y& operator=(initializer_list<value_type> il) {}

		iterator begin() noexcept {
			return start;
		}
		const_iterator begin() const noexcept {
			return start;
		}
		const_iterator cbegin() const {
			return static_cast<const_iterator>(start);
		}

		iterator end() noexcept {
			return finish;
		}
		const_iterator end() const noexcept {
			
		}
		const_iterator cend() const {
			return static_cast<const_iterator>(finish);
		}

		size_type size() const noexcept{
			return finish - start;
		}
		size_type max_size() const noexcept{

		}
		size_type capacity() const noexcept{
			return end_of_storage - start;
		}
		bool empty() const noexcept{
			return 0 == size();
		}

		void resize(size_type n, value_type val = value_type()) {
			if (n < size())
				finish = start + n;
			else if (n > size()) {
				insert(end(), n - size(), val);
			}
		}

		void reserve(size_type n) {
			if (n > capacity()) {
				auto new_start = data_allocator.allocate(n);
				uninitialized_copy_y(start, finish, new_start);

				_destroy_y(start, finish);
				dealloate();

				finish = new_start + (finish - start);
				start = new_start;
				end_of_storage = n;
			}
		}
		void shrink_to_fit() {
			if (size() != capacity()) {
				auto new_start = data_allocator.allocate(size());
				uninitialized_copy_y(start, finish, new_start);

				_destroy_y(start, finish);
				dealloate();

				end_of_storage = finish = new_start + (finish - start);
				start = new_start;
			}
		}

		void push_back(const value_type& value) {
			if (finish < end_of_storage) {
				_construct_y(finish++, value);
			} else {
				insert(end(), value);
			}
		}
		void push_back(value_type&& value) {

		}
		void pop_back() {
			_destroy_y(--finish);
		}

		iterator insert(const iterator position, const value_type& value) {
			auto p = insert_aux(position, 1);
			if (p.second > 0)
				*p.first = value;
			else
				_construct_y(p.first, value);
			return p;
		}
		iterator insert(const iterator position, size_type n, const value_type& value) {
			auto p = insert_aux(position, n);
			fill_n(p.first, p.second, value);
			uninitialized_fill_n_y(p.first + p.second, n - p.second, value);
			return p;
		}
		template <class InputIterator>
		iterator insert(const iterator position, InputIterator first, InputIterator last) {
			auto p = insert_aux(position, last - first);
			copy(first, first + p.second, p.first);
			uninitialized_copy_y(first + p.second, last, p.first + p.second);
			return p;
		}
		iterator insert(const iterator position, value_type&& value) {

		}
		iterator insert(const iterator position, initializer_list<value_type> il) {
			return insert(position, il.begin(), il.end());
		}

		iterator erase(const_iterator position) {
			copy(position + 1, end(), position);
			--finish;
			_destroy_y(finish);
			return position;
		}
		iterator erase(const_iterator first, const_iterator last) {
			auto i = copy(last, end(), first);
			_destroy_y(i, finish);
			finish = i;
			return first;
		}




	private:
		Alloc data_allocator;
		iterator start = nullptr;
		iterator finish = nullptr;
		iterator end_of_storage = nullptr;
	
	private:
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		iterator allocate_and_fill(size_type n, const T& value) {
			auto start = data_allocator.allocate(n);
			uninitialized_fill_n_y(start, n, value);
			return start;
		}

		pair_y<iterator, size_type> insert_aux(iterator position, size_type n) {
			const size_type elem_after = finish - position;
			if (n + finish <= end_of_storage) {
				if (elem_after > n) {
					uninitialized_copy_y(finish - n, finish, finish);
					copy_backward(position, finish - n, finish);
					finish += n;
					return {position, n};
				} else {
					uninitialized_copy_y(position, finish, position + n)
					auto constructed_size = finish - position;
					finish = position + n;
					return {position, constructed_size};
				}
			} else {
				auto old_size = size();
				auto new_size = max(old_size * 2, old_size + n);
				auto new_start = data_allocator.allocate(new_size);
				auto elem_prev = position - start;

				uninitialized_copy_y(start, position, new_start);
				uninitialized_copy_y(position, finish, new_start + elem_prev + n);

				_destroy_y(begin(), end());
				deallocate();

				end_of_storage = new_start + new_size;
				start = new_start;
				finish = new_start + old_size + n;

				return {start + elem_prev, 0};
			}
		}

		void dealloate() {
			if (start)
				data_allocator.deallocate(start, capacity());
		}
	};
}