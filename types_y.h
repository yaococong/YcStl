#ifndef TYPES_H
#define TYPES_H
#include <cstddef>

namespace yc {
	using size_t = unsigned int;

	struct input_iterator_tag_y {};
	struct output_iterator_tag_y {};
	struct forward_iterator_tag_y : public input_iterator_tag_y {};
	struct bidirectional_iterator_tag_y : public forward_iterator_tag_y {};
	struct random_iterator_tag_y : public bidirectional_iterator_tag_y {};

	template <class Category, class ValueType, class Distance = std::ptrdiff_t, class Pointer = ValueType*, class Refercnce = ValueType&>
	class iterator_y {
		using iterator_category = Category;
		using value_type = ValueType;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Refercnce;
	};

	template <class Iterator>
	struct iterator_traits_y {
		using iterator_category = typename Iterator::iterator_category;
		using value_type = typename Iterator::value_type;
		using difference_type = typename Iterator::difference_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
	};

	template <class T>
	struct iterator_traits_y<T*> {
		using iterator_category = random_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	};

	template <class T>
	struct iterator_traits_y<const T*> {
		using iterator_category = random_iterator_tag_y;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
	};

	template <class Iterator>
	inline typename iterator_traits_y<Iterator>::value_type* value_type_y(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
	}

	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category category_type_y(const Iterator&) {
		return typename iterator_traits<Iterator>::iterator_category();
	}

	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type* difference_type_y(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	struct __true_type_y {};
	struct __false_type_y {};

	template <class type>
	struct __type_traits_y {
		using this_dummy_member_must_be_first = __true_type_y;
		using has_trivial_default_constructor = __false_type_y;
		using has_trivial_copy_constructor = __false_type_y;
		using has_trivial_assignment_operator = __false_type_y;
		using has_trivial_destructor = __false_type_y;
		using is_POD_type = __false_type_y;
	};

	// template <class allocator_type>
	// struct allocator_traits {
	// 	using pointer = allocator_type::pointer;
	// 	using const_pointer = 
	// };
}

#endif