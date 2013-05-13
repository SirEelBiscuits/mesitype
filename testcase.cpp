template<typename T>
struct Foo {
	T val;
	constexpr explicit Foo(T const in) : val(in) {}
};

int main() {
	Foo<int> constexpr Bar(3);
}

