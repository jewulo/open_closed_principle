#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace open_closed
{
	enum class COLOR { RED, GREEN, BLUE };
	enum class SIZE { SMALL, MEDIUM, LARGE };

	struct Product {
		std::string m_name;
		COLOR		m_color;
		SIZE		m_size;
	};

	using Items = std::vector<Product*>;
	//#define ALL(C) begin(C), end(C)

	template <typename T>
	struct Specification {
		virtual ~Specification() = default;
		virtual bool is_satisfied(T* item) const = 0;
	};

	struct ColorSpecification : Specification<Product> {
		COLOR e_color;
		ColorSpecification(COLOR e_color) : e_color(e_color) {}
		bool is_satisfied(Product* item) const {
			return item->m_color == e_color;
		}
	};

	struct SizeSpecification : Specification<Product> {
		SIZE e_size;
		SizeSpecification(SIZE e_size) : e_size(e_size) {}
		bool is_satisfied(Product* item) const {
			return item->m_size == e_size;
		}
	};

	template <typename T>
	struct Filter {
		virtual std::vector<T*> filter(std::vector<T*> items, const Specification<T> &spec) = 0;
	};

	struct BetterFilter : Filter<Product> {
		std::vector<Product*> filter(std::vector<Product*> items, const Specification<Product> &spec) {
			std::vector<Product*> result;
			for (auto& p : items)
				if (spec.is_satisfied(p))
					result.push_back(p);
			return result;
		}
	};

	template <typename T>
	struct AndSpecification : Specification<T> {
		const Specification<T>& first;
		const Specification<T>& second;

		AndSpecification(const Specification<T> &first, const Specification<T>& second)
			: first(first), second(second)
		{}

		bool is_satisfied(T* item) const {
			return first.is_satisfied(item) && second.is_satisfied(item);
		}
	};

	template <typename T>
	AndSpecification<T> operator&&(const Specification<T>& first, const Specification<T>& second) {
		return { first, second };	// implicit conversion to constructor AndSpecification(first, second)
	}

	void run() {
		const Items all {
			new Product{"Apple", COLOR::GREEN, SIZE::SMALL},
			new Product{"Tree", COLOR::GREEN, SIZE::LARGE},
			new Product{"House", COLOR::BLUE, SIZE::LARGE},
		};

		BetterFilter bf;
		for (auto &x : bf.filter(all, ColorSpecification(COLOR::GREEN)))
			std::cout << x->m_name << " is green\n";

		auto green_things = ColorSpecification{ COLOR::GREEN };
		auto large_things = SizeSpecification{ SIZE::LARGE };
		for (auto& x : bf.filter(all, green_things && large_things))
			std::cout << x->m_name << " is green and large\n";
	}
}
