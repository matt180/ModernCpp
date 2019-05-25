#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <list>


struct indexed {

	int start;

	indexed(int n = 0) : start(n) { }
};

template <template <class, class...> class Cont, class T, class ...E>
class IndexedIterator {

	private:
		typename Cont<T, E...>::iterator iter;
		int i;

	public:

		IndexedIterator(const typename Cont<T, E...>::iterator& it, int n = 0) : iter{ it }, i{ n } { }

		T& value() const {
			return *iter;
		}

		int index() const {
			return i;
		}

		IndexedIterator operator++() {
			++iter;
			++i;
			return *this;
		}

		IndexedIterator& operator*() {
			return *this;
		}

		bool operator!=(const IndexedIterator& rhs) {
			return !(iter == rhs.iter && i == rhs.i);
		}
};




template <template <class, class...> class Cont, class T, class... E>
class Indexed {
	private:
		int start;
		int last;
        const typename Cont<T, E...>::iterator beg, en;

	public:
		Indexed(int n, int l, const typename Cont<T, E...>::iterator& b, const typename Cont<T, E...>::iterator& e)
			: start{ n }, last{ l }, beg{ b }, en{ e } { }

		const IndexedIterator<Cont, T, E...> begin() const {
			return IndexedIterator<Cont, T, E...>(beg, start);
		}

		const IndexedIterator<Cont, T, E...> end() const {
			return IndexedIterator<Cont, T, E...>(en, last);
		}

};

template <template <class, class...> class Cont, class T, class ...E>
Indexed<Cont, T, E...> operator|(Cont<T, E...>& cont, indexed ind) {
	return Indexed<Cont, T, E...>(ind.start, cont.size() + ind.start, cont.begin(), cont.end());
}

int main()
{
	std::list<std::string> szavak = { "alma", "korte", "barack" };
    for (auto const& v : szavak | indexed(1)) {
		std::cout << v.index() << " " << v.value() << std::endl;
	}
}
