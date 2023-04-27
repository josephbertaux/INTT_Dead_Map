#ifndef HASH_ITERATOR_H
#define HASH_ITERATOR_H

#include <iostream>

class hash_iterator;

bool operator<(hash_iterator const&, hash_iterator const&);
bool operator>(hash_iterator const&, hash_iterator const&);
bool operator<=(hash_iterator const&, hash_iterator const&);
bool operator>=(hash_iterator const&, hash_iterator const&);
bool operator==(hash_iterator const&, hash_iterator const&);
bool operator!=(hash_iterator const&, hash_iterator const&);

class hash_iterator
{
public:
	typedef int HASH;
	typedef int SHIFT;

	typedef SHIFT(*_NUM_FIELDS)();
	typedef HASH(*_MAX_VAL)(SHIFT const&);
	typedef HASH(*_LAST_VAL)(SHIFT const&, HASH const&, SHIFT* const&, SHIFT* const&);

	hash_iterator();
	hash_iterator(hash_iterator const&);
	hash_iterator(hash_iterator const&, HASH const&);
	~hash_iterator();

	HASH& operator*();
	hash_iterator& operator++();
	hash_iterator& operator--();

	friend bool operator<(hash_iterator const&, hash_iterator const&);
	friend bool operator>(hash_iterator const&, hash_iterator const&);
	friend bool operator<=(hash_iterator const&, hash_iterator const&);
	friend bool operator>=(hash_iterator const&, hash_iterator const&);
	friend bool operator==(hash_iterator const&, hash_iterator const&);
	friend bool operator!=(hash_iterator const&, hash_iterator const&);

	hash_iterator begin();
	hash_iterator last();
	hash_iterator end();

	SHIFT size() const;
	SHIFT num_fields() const;

	HASH get_hash() const;
	HASH set_hash(HASH const&);

	static HASH get_field(SHIFT const&, HASH const&, SHIFT* const&, SHIFT* const&);

	HASH get_field(SHIFT const&) const;
	HASH set_field(SHIFT const&, HASH const&);

	virtual HASH pseudo_hash() const;
	virtual HASH phi_hash() const;
	virtual HASH r_hash() const;
	virtual HASH z_hash() const;

protected:
	hash_iterator(_NUM_FIELDS const&, _MAX_VAL const&, _LAST_VAL const&);

	SHIFT NUM_FIELDS = 0;

	SHIFT* bit_wdths = nullptr;
	SHIFT* bit_shfts = nullptr;
	_LAST_VAL LAST_VAL = nullptr;

	SHIFT SIZE = 1;

	HASH BEGIN_HASH = 0;
	HASH LAST_HASH = 0;
	HASH END_HASH = 1;

	HASH hash = 0;
};

#endif
