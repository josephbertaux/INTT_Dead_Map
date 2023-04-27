#include <iostream>

#include "intt_hash.hpp"

//void print_hash_value(intt_hash const&);
void print_hash_value(intt_hash&);
void print_hash_indexes(intt_hash const&);

int main()
{
	intt_hash my_hash(2, 15, 1, 25, 127, 7);

	print_hash_indexes(my_hash);
	std::cout << "inc" << std::endl;
	intt_hash foo = ++my_hash;
	print_hash_indexes(my_hash);
	std::cout << "dec" << std::endl;
	intt_hash bar = --my_hash;
	print_hash_indexes(my_hash);

	std::cout << "layer" << my_hash.get_field(intt_hash::i_lyr) << std::endl;
	std::cout << "equal: " << (my_hash == my_hash) << std::endl;
	std::cout << "not equal: " << (foo == bar) << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;

	my_hash = intt_hash(my_hash.end());
	std::cout << "should be end:" << std::endl;
	print_hash_value(my_hash);

	std::cout << "should be last:" << std::endl;
	--my_hash;
	print_hash_value(my_hash);
	print_hash_indexes(my_hash);

	std::cout << "should be adjacent:" << std::endl;
	--my_hash;
	print_hash_value(my_hash);
	print_hash_indexes(my_hash);

	std::cout << "pseudo hash:" << std::endl;
	my_hash = intt_hash(my_hash.pseudo_hash());
	print_hash_value(my_hash);
	print_hash_indexes(my_hash);

	std::cout << "directly inc hash value:" << std::endl;
	print_hash_value(my_hash);
	++*my_hash;
	print_hash_value(my_hash);
	std::cout << std::hex << *my_hash << std::endl;

	return 0;
}

//void print_hash_value(intt_hash const& _h)
void print_hash_value(intt_hash& _h)
{
	//printf("%#0*x\n", _h.size() / 4 + 3, _h.get_hash());
	printf("%#0*x\n", _h.size() / 4 + 3, *_h);
}

void print_hash_indexes(intt_hash const& _h)
{
	auto a = _h.get_indexes();
	for(int i = 0; i < a.size(); ++i)
	{
		std::cout << a[i] << std::endl;
	}
}
