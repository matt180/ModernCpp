#include <iostream>
#include <memory>
#include <cmath>
#include <type_traits>
#include <functional>


template<typename T>
class my_function;

template <typename Ret, typename ... Args>
class my_function<Ret(Args...)> {

public:

    my_function() : func_(nullptr) { }

    my_function(const my_function& other) : func_(other.func_->clone()) {}

    template <typename F>
	my_function& operator=(F f) {
		func_ = std::make_unique<Functor<F>>(f);
		return *this;
	}

	Ret operator()(Args... args) const {
	    if (func_ == nullptr)
            throw std::bad_function_call();
		return func_->operator()(args...);
	}

    operator bool() const {
        return func_ != nullptr;
    }
private:

	class FunctorBase {
	public:
		virtual ~FunctorBase() = default;
		virtual Ret operator()(Args...) = 0;
		virtual std::unique_ptr<FunctorBase> clone() = 0;
	};

    template <typename F>
	class Functor : public FunctorBase {
	public:


		Functor(const F& f)
			: f_(f) {
		}

		Ret operator()(Args... args) override {
			return helper<F>(args...);
		}

		std::unique_ptr<FunctorBase> clone() override {
            return std::make_unique<Functor<F>>(f_);
		}

	private:

        template <typename T, typename std::enable_if<!std::is_null_pointer<T>::value>::type* = nullptr>
		Ret helper(Args... args) {
            return f_(args...);
		}

		template <typename T, typename std::enable_if<std::is_null_pointer<T>::value>::type* = nullptr>
		Ret helper(Args...) {
		    throw std::bad_function_call();
		}

		F f_;

	};

    std::unique_ptr<FunctorBase> func_;
};



int main()
{
    my_function<double(double)> f{};

    if (!f)
        std::cout << "Egyelõre nullptr" << std::endl;

    f = sin;
    std::cout << sin(2.3) << "==" << f(2.3) << std::endl;

    f = [] (double x) { return x*x; };
    std::cout << 2.3*2.3 << "==" << f(2.3) << std::endl;

    f = std::bind(pow, std::placeholders::_1, 4);
    std::cout << pow(2.3, 4) << "==" << f(2.3) << std::endl;

    auto f2 = f;
    std::cout << pow(2.3, 4) << "==" << f2(2.3) << std::endl;

    f = nullptr;
    try {
        f(2.3);
    } catch (std::bad_function_call &e) {
        std::cout << "Megint nullptr" << std::endl;
    }
    return 0;
}

