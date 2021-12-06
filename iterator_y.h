#ifndef ITERATOR_H
#define ITERATOR_H

#include "types.h"

namespace yc {
	template <class InputIterator>
	inline void __advance_y(InputIterator &iterator, int n, input_iterator_tag_y) {
		while (n--)
			iterator++;
	}
	template <class InputIterator>
	inline void __advance_y(InputIterator &iterator, int n, bidirectional_iterator_tag_y) {
		if (n > 0) {
			while (n--)
				iterator++;
		} else {
			while (n++)
				iterator--;
		}
	}
	template <class InputIterator>
	inline void __advance_y(InputIterator &iterator, int n, random_iterator_tag_y) {
		return iterator += n;
	}
	template <class InputIterator>
	inline void advance_y(InputIterator &iterator, int n) {
		__advance_y(iterator, n, iterator_category(iterator));
	}

	template <class InputIterator>
	inline typename iterator_traits_y<InputIterator>::difference_type __difference_y(InputIterator first, InputIterator last, input_iterator_tag_y) {
		typename iterator_traits<InputIterator>::difference_type ans = 0;
		for (; first != last; ++first, ++ans)
			;
		return ans;
	}

	template <class InputIterator>
	inline typename iterator_traits_y<InputIterator>::difference_type __difference_y(InputIterator first, InputIterator last, random_iterator_tag_y) {
		return last - first;
	}

	template <class InputIterator>
	inline typename iterator_traits_y<InputIterator>::difference_type difference_y(InputIterator first, InputIterator last) {
		return __difference_y(first, last, iterator_category(InputIterator));
	}
}

#endif