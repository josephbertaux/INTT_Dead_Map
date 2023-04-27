#include "test.hpp"

void print_hash(my_itr_class const&);
void print_fields(my_itr_class const&);

int main()
{
	my_itr_class itr;
	std::cout << itr.size() << std::endl;
	std::cout << itr.num_fields() << std::endl;

	std::cout << "looping over values..." << std::endl;

	print_hash(itr);
	print_fields(itr);
	while(++itr != itr.end())
	{
		print_hash(itr);
		print_fields(itr);
	}

	print_hash(itr);
	print_fields(itr);

	return 0;
}

void print_hash(my_itr_class const& _m)
{
	printf("%#0*x\n", _m.size() / 4 + 2, _m.get_hash());
}

void print_fields(my_itr_class const& _m)
{
	for(my_itr_class::SHIFT s = 0; s < _m.num_fields(); ++s)
	{
		printf("\t%#0*x\n", _m.size() / 4 + 2, _m.get_field(s));
	}
}
