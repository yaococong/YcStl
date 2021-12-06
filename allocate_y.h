#ifndef ALLOCATE_H
#define ALLOCATE_H

#include "types_y.h"
#include <new>
#include <climits>
#include <utility>
#include <algorithm>

#if 0
# include <new>
# define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
# include <iostream>
# define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; std::exit(1)
#endif

namespace yc {
	template <class T>
	inline T* _allocate_y(size_t size, T*) {
		std::set_new_handler(nullptr);
		return static_cast<T*>(::operator new(size * sizeof(T)));
		
	}

	template <class T>
	inline void _deallocate_y(T* p) {
		::operator delete(p);
	}

	template <class T, class... Args>
	inline void _construct_y(T* p, Args... args) {
		new(p) T(std::forward<Args>(args)...);
	}

	template <class T>
	inline void _destroy_y(T* p) {
		p->~T();
	}

	template <class ForwardIterator>
	inline void _destroy_y(ForwardIterator first, ForwardIterator last) {
		using value_type = typename iterator_traits<ForwardIterator>::value_type;
		__destroy_y(first, last, typename __type_traits<value_type>::has_trivial_destructor());
	}

	inline void _destroy_y(char *, char *) {}
	inline void _destroy_y(wchar_t *, wchar_t *) {}

	template <class ForwardIterator>
	inline void __destroy_y(ForwardIterator first, ForwardIterator last, __true_type) {}

	template <class ForwardIterator>
	inline void __destroy_y(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first != last; ++first)
			_destroy_y(&*first);
	}

	template <class T, class Alloc>
	class simple_alloc_y {
	public:
		static T* allocate(size_t n) {
			return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
		}
		static T* allocate() {
			return static_cast<T*>(Alloc::allocate(sizeof(T)));
		}
		
		static void deallocate(T *p, size_t n) {
			if (0 != n)
				Alloc::deallocate(p, n * sizeof(T));
		}
		static void deallocate(T *p) {
			Alloc::deallocate(p, sizeof(T));
		}
	};

	template <class T>
	class allocator_y {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef __true_type propagate_on_containter_move_assignment;

		template <class U>
		struct rebind {
			typedef allocator_y<U> other;
		};

		allocator_y() noexcept = default;
		allocator_y(const allocator_y& alloc) noexcept = default;
		template <class U>
		allocator_y(const allocator_y<U>& alloc) noexcept {}

		pointer allocate(size_type n, const void* = nullptr) { return _allocate_y(n, pointer()); }
		void deallocate(pointer p, size_type n = 1) { _deallocate_y(p); }

		pointer address(reference x) const { return &x; }
		const_pointer address(const_reference x) const { return &x; }

		size_type max_size() { return size_type(UINT_MAX / sizeof(T)); }
		
		template<class... Args>
		void construct(pointer p, Args... args) { _construct_y(p, std::forward<Args>(args)...); }
		
		void destroy(pointer p) { _destroy_y(p); }
	};

	template <int inst>
	class __malloc_alloc_template_y {
	private:
		static void *oom_malloc(size_t);
		static void *oom_remalloc(void *, size_t);
		static void (* __malloc_alloc_oom_handler)();

	public:
		static void* allocate(size_t n) {
			void *ans = malloc(n);
			if (0 == ans) 
				return oom_malloc(n);
		}

		static void deallocate(void *p, size_t n) {
			free(p);
		}

		static void* reallocate(void *p, size_t n) {
			void *ans = realloc(p, n);
			if (0 == ans)
				return oom_remalloc(p, n);
		}

		static void (* set_malloc_handler(void (*f)()))() {
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return old;
		}
	};

	template <int inst>
	void (*__malloc_alloc_template_y<inst>::__malloc_alloc_oom_handler)() = 0;

	template <int inst>
	void *__malloc_alloc_template_y<inst>::oom_malloc(size_t n) {
		for (;;) {
			if (0 == __malloc_alloc_oom_handler)
				__THROW_BAD_ALLOC;
			(*__malloc_alloc_oom_handler)();
			void *ans = malloc(n);
			if (ans)
				return ans;
		}
	}

	template <int inst>
	void *__malloc_alloc_template_y<inst>::oom_remalloc(void *p, size_t n) {
		for (;;) {
			if (0 == __malloc_alloc_oom_handler)
				__THROW_BAD_ALLOC;
			(*__malloc_alloc_oom_handler)();
			void *ans = realloc(p, n);
			if (ans)
				return ans;
		}
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy_y(InputIterator first, InputIterator last, ForwardIterator result) {
		using value_type = typename iterator_traits<ForwardIterator>::value_type;
		return __uninitialized_copy_y(first, last, result, typename __type_traits<value_type>::is_POD_type());
	}
	template<class InputIterator, class ForwardIterator>
	ForwardIterator __uninitialized_copy_y(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
		return std::copy(first, last, result);
	}
	template<class InputIterator, class ForwardIterator>
	ForwardIterator __uninitialized_copy_y(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
		auto cur = result;
		for (; first != last; ++first, ++cur)
			_construct_y(&*cur, *first);
		return cur;
	}

	template<class ForwardIterator, class T>
	void uninitialized_fill_y(ForwardIterator first, ForwardIterator last, const T& x) {
		using value_type = typename iterator_traits<ForwardIterator>::value_type;
		__uninitialized_fill_y(first, last, x, typename __type_traits<value_type>::is_POD_type());
	}
	template<class ForwardIterator, class T>
	void __uninitialized_fill_y(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
		std::fill(first, last, x);
	}
	template<class ForwardIterator, class T>
	void __uninitialized_fill_y(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
		for (; first != last; ++first)
			_construct_y(&*first, x);
	}

	template<class ForwardIterator, class Size, class T>
	void uninitialized_fill_n_y(ForwardIterator first, Size n, const T& x) {
		using value_type = typename iterator_traits<ForwardIterator>::value_type;
		__uninitialized_fill_n(first, n, x, typename __type_traits<value_type>::is_POD_type());
	}
	template<class ForwardIterator, class Size, class T>
	void uninitialized_fill_n_y(ForwardIterator first, Size n, const T& x, __true_type) {
		std::fill_n(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	void uninitialized_fill_n_y(ForwardIterator first, Size n, const T& x, __false_type) {
		for (; n > 0; --n, ++first)
			_construct_y(&*first, x);
	}
}

#endif