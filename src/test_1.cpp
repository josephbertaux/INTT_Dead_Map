#include <iostream>
#include <memory>

#include "hash_iterator.hpp"
#include "intt_iterator.hpp"

void print_hash(intt_iterator const&);
void print_fields(intt_iterator const&);

int main()
{
	//std::shared_ptr<hash_iterator> itr = std::make_shared<intt_iterator>();
	intt_iterator itr;
	std::cout << itr.size() << std::endl;
	std::cout << itr.num_fields() << std::endl;

	std::cout << "checking specific increments:" << std::endl;
	std::cout << "first test:" << std::endl;
	*itr = 0;
	itr.set_field(intt_iterator::i_lyr, 2);
	print_hash(itr);
	print_fields(itr);
	std::cout << "dec:" << std::endl;
	--itr;
	print_hash(itr);
	print_fields(itr);
	std::cout << "inc:" << std::endl;
	++itr;
	print_hash(itr);
	print_fields(itr);

	std::cout << "second test:" << std::endl;
	*itr = 0;
	itr.set_field(intt_iterator::i_lyr, 3);
	print_hash(itr);
	print_fields(itr);
	std::cout << "dec:" << std::endl;
	--itr;
	print_hash(itr);
	print_fields(itr);
	std::cout << "inc:" << std::endl;
	++itr;
	print_hash(itr);
	print_fields(itr);

	--itr;
	std::cout << "checking pseudo hash" << std::endl;
	std::cout << std::hex << itr.get_hash() << std::endl;
	std::cout << std::hex << itr.pseudo_hash() << std::endl;
	std::cout << std::hex << itr.get_hash() << std::endl;

	//print_hash(itr);
	//print_fields(itr);
	//while(++itr != itr.end())
	//{
	//	print_hash(itr);
	//	print_fields(itr);
	//}

	//print_hash(itr);
	//print_fields(itr);

	return 0;
}

void print_hash(intt_iterator const& _m)
{
	printf("%#0*x\n", _m.size() / 4 + 3, _m.get_hash());
}

void print_fields(intt_iterator const& _m)
{
	for(intt_iterator::SHIFT s = 0; s < _m.num_fields(); ++s)
	{
		printf("\t%#0*x\n", _m.size() / 4 + 3, _m.get_field(s));
	}
}
