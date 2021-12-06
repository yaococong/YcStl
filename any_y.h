#ifndef ANY_H
#define ANY_H

#include <cassert>
#include <typeinfo>
namespace yc {
	class any_y {
		class placeholder {
		public:
			virtual ~placeholder() {}
			virtual const std::type_info& type() const = 0;
			virtual placeholder* clone() const = 0;
		};

		template <class ValueType>
		class holder : public placeholder {
		public:
			ValueType value;
		public:
			holder(const ValueType& val) : value(val) {}
			const std::type_info& type() const override {
				return typeid(ValueType);
			}
			placeholder* clone() const override {
				return new holder(value);
			}
		};

		template <class ValueType>
		friend ValueType any_cast_y(const any_y& a);
	public:
		any_y() = default;
		template <class ValueType>
		any_y(const ValueType &val) : contain(new holder<ValueType>(val)) {}
		any_y(const any_y &rhs) : contain(rhs.contain->clone()) {}

		~any_y() {
			delete contain;
		}

		template <class ValueType>
		any_y& operator=(const ValueType &val) {
			delete contain;
			contain = new holder<ValueType>(val);
			return *this;
		}
		any_y& operator=(const any_y& rhs) {
			delete contain;
			contain = rhs.contain->clone();
			return *this;
		}

		const std::type_info& type() const {
			return contain ? contain->type() : typeid(void);
		}
	private:
		placeholder *contain = nullptr;
	};

	template <class ValueType>
	ValueType any_cast_y(const any_y& a) {
		assert(typeid(ValueType) == a.contain->type());
		return static_cast<typename any_y::holder<ValueType>*>(a.contain)->value;
	}
}

#endif