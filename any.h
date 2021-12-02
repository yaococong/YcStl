#include <cassert>
#include <typeinfo>

class any {
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
	friend ValueType any_cast(const any& a);
public:
	any() = default;
	template <class ValueType>
	any(const ValueType &val) : contain(new holder<ValueType>(val)) {}
	any(const any &rhs) : contain(rhs.contain->clone()) {}

	~any() {
		delete contain;
	}

	template <class ValueType>
	any& operator=(const ValueType &val) {
		delete contain;
		contain = new holder<ValueType>(val);
		return *this;
	}
	any& operator=(const any& rhs) {
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
ValueType any_cast(const any& a) {
	assert(typeid(ValueType) == a.contain->type());
	return static_cast<typename any::holder<ValueType>*>(a.contain)->value;
}