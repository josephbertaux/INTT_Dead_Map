#include <iostream>
#include <string>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TH2D.h"

#include "hash_iterator.hpp"
#include "intt_iterator.hpp"

struct BranchMapStruct
{
	intt_iterator::SHIFT index;
	Long64_t count;
};

struct HashMapStruct
{
	Long64_t count;			//the counts at this hash
	intt_iterator::HASH hashes;	//the number of UNIQUE hashes contributing to the count
};

typedef std::map<std::string, BranchMapStruct> BranchMap_t;
typedef std::map<hash_iterator::HASH, HashMapStruct> HashMap_t;

//for a map of maps maintaing stratified statistics
typedef intt_iterator::HASH(intt_iterator::*StrataFunc_t)() const;
struct StrataMapStruct
{
	StrataFunc_t func;
	HashMap_t map;
};
typedef std::map<std::string, StrataMapStruct> StrataMap_t;

void print_hash(intt_iterator const&);
void print_fields(intt_iterator const&);

int main(int argc, char** argv)
{
	//////////////////////////////////
	//	Variable Declarations	//
	//////////////////////////////////

	//Identify the brances you expect to find in the tree
	//Only the keys need to be correct
	//I am assume they are all of a common type (e.g., Int_t, Long64_t)
	BranchMap_t branches =
	{
		{"lyr", (BranchMapStruct){.index = intt_iterator::i_lyr, .count = 0}},
		{"ldr", (BranchMapStruct){.index = intt_iterator::i_ldr, .count = 0}},
		{"nos", (BranchMapStruct){.index = intt_iterator::i_nos, .count = 0}},
		{"chp", (BranchMapStruct){.index = intt_iterator::i_chp, .count = 0}},
		{"chn", (BranchMapStruct){.index = intt_iterator::i_chn, .count = 0}}
	};

	//append the list of maps you want to do stratified analysis on
	//only one map tracking performance based on pseudorapidity should be enough
	//you need to change the memember function pointer to the one you want to use
	//that returns the stratified hash
	StrataMap_t strata_map =
	{
		{"pseudo", (StrataMapStruct){.func = &intt_iterator::pseudo_hash, .map = {}}}
	};

	Long64_t n = 0;

	TFile* i_file = nullptr;
	TTree* i_tree = nullptr;

	TFile* o_file = nullptr;
	TTree* o_tree = nullptr;

	std::string i_file_name = "";
	std::string i_tree_name = "tree";

	std::string o_file_name = "";
	std::string o_tree_name = "";

	HashMap_t raw_counts;

	BranchMap_t::iterator branch_itr;
	StrataMap_t::iterator strata_itr;
	intt_iterator intt_itr;

	//////////////////////////
	//	Guard Clauses	//
	//	Initializations	//
	//////////////////////////

	if(argc < 2)
	{
		std::cout << "You must specify the name of a root file relative to the executable path" << std::endl;
		return 1;
	}
	i_file_name = argv[1];
	if(argc > 3)
	{
		i_tree_name = argv[2];
		std::cout << "Using tree \'" << i_tree_name << "\'" << std::endl;
	}

	o_file_name = i_file_name;
	try
	{
		o_file_name.insert(i_file_name.find(".root"), "_o");
	}
	catch(const std::exception& e)
	{
		std::cout << "File name \'" << i_file_name << "\' does not contain the suffix \'.root\'" << std::endl;
		return 1;
	}
	o_tree_name = i_tree_name;

	i_file = TFile::Open(i_file_name.c_str());
	if(!i_file)
	{
		std::cout << "Could not open file:" << std::endl;
		std::cout << "\t" << i_file_name << std::endl;
		return 1;
	}
	i_tree = (TTree*)i_file->Get(i_tree_name.c_str());
	if(!i_tree)
	{
		std::cout << "Could not get tree \'" << i_tree_name << "\' from file:" << std::endl;
		std::cout << "\t" << i_file_name << std::endl;
		return 1;
	}
	bool flag = false;
	for(branch_itr = branches.begin(); branch_itr != branches.end(); ++branch_itr)
	{
		if(!i_tree->GetBranch((branch_itr->first).c_str()))
		{
			std::cout << "Could not get branch \'" << branch_itr->first << "\' from tree" << std::endl;
			flag = true;
			continue;
		}

		i_tree->SetBranchAddress((branch_itr->first).c_str(), &(branch_itr->second.count));

	}
	if(flag)return 1;

	//////////////////////////////////
	//	Main Implementations	//
	//////////////////////////////////

	//copy the data from the tree into our maps
	for(n = 0; n < i_tree->GetEntriesFast(); ++n)
	{
		i_tree->GetEntry(n);

		for(branch_itr = branches.begin(); branch_itr != branches.end(); ++branch_itr)
		{
			intt_itr.set_field(branch_itr->second.index, branch_itr->second.count);
		}

		//check the strata maps if their hash is present
		for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
		{
			//if the stratified hash is absent,
			if(strata_itr->second.map.find((intt_itr.*(strata_itr->second.func))()) == strata_itr->second.map.end())
			{
				//intialize the element at the stratified hash
				strata_itr->second.map[(intt_itr.*(strata_itr->second.func))()] = (HashMapStruct){.count = 0, .hashes = 0};
			}
		}

		//check the main/raw map for the unique hash
		if(raw_counts.find(intt_itr.get_hash()) == raw_counts.end())
		{
			//if it was absent, increment the count of unique hashes for the strata map
			for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
			{
				++strata_itr->second.map[(intt_itr.*(strata_itr->second.func))()].hashes;
			}

			//initialize the element at the unique hash for the main//raw map
			//there will only ever be a single unique hash contributing to the main/raw map element
			raw_counts[intt_itr.get_hash()] = (HashMapStruct){.count = 0, .hashes = 1};
		}

		//increment the counts for the stratified hashes
		for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
		{
			++strata_itr->second.map[(intt_itr.*(strata_itr->second.func))()].count;
		}
		++raw_counts[intt_itr.get_hash()].count;
	}

	//Now perform analysis on the stratified maps

	std::cout << "raw counts:" << std::endl;
	for(auto itr = raw_counts.begin(); itr != raw_counts.end(); ++itr)
	{
		std::cout << "\t" << std::hex << itr->first;
		std::cout << " :\t" << itr->second.count << std::endl;
	}

	for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
	{
		std::cout << strata_itr->first << std::endl;
		for(auto itr = strata_itr->second.map.begin(); itr != strata_itr->second.map.end(); ++itr)
		{
			std::cout << "\t" << std::hex << itr->first;
			std::cout << " :\t" << itr->second.count;
			std::cout << " :\t" << itr->second.hashes << std::endl;
		}
	}

	return 0;
}









//In-house global functions


//////////////////////////
//	For Debugging	//
//////////////////////////

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
