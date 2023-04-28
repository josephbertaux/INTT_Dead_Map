#include <iostream>
#include <string>
#include <map>
#include <chrono>

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
typedef std::map<std::string, BranchMapStruct> BranchMap_t;

struct HashMapStruct
{
	Long64_t count;
	intt_iterator::HASH hashes;
	Double_t mean;
	Double_t sigma;
};
typedef std::map<hash_iterator::HASH, HashMapStruct> HashMap_t;

struct RawMapStruct
{
	Long64_t count;
	intt_iterator::HASH hashes;
	std::map<std::string, Double_t> strata_z;
};
typedef std::map<hash_iterator::HASH, RawMapStruct> RawMap_t;

typedef intt_iterator::HASH(intt_iterator::*StrataFunc_t)() const;
struct StrataMapStruct
{
	StrataFunc_t func;
	HashMap_t map;
};
typedef std::map<std::string, StrataMapStruct> StrataMap_t;

//////////////////////////////////////////
//	Global function implementations	//
//////////////////////////////////////////

template <typename T_i, typename T_I, typename T_p, typename T_P>
void show_progress(T_i&, T_I const&, T_p const&, T_P const&, std::chrono::high_resolution_clock::time_point&);

void print_hash(intt_iterator const&);

void print_fields(intt_iterator const&);

//////////////////
//	Main	//
//////////////////

int main(int argc, char** argv)
{
	//////////////////////////////////
	//	Variable Declarations	//
	//////////////////////////////////

	//Identify the brances you expect to find in the tree
	//make sure the key and the enum in the Struct correspond
	BranchMap_t branches =
	{
		{"lyr", (BranchMapStruct){.index = intt_iterator::i_lyr, .count = 0}},
		{"ldr", (BranchMapStruct){.index = intt_iterator::i_ldr, .count = 0}},
		{"nos", (BranchMapStruct){.index = intt_iterator::i_nos, .count = 0}},
		{"chp", (BranchMapStruct){.index = intt_iterator::i_chp, .count = 0}},
		{"chn", (BranchMapStruct){.index = intt_iterator::i_chn, .count = 0}}
	};

	//append the list of maps you want to do stratified analysis on
	//make sure the key and the member function in the struct correspond
	StrataMap_t strata_map =
	{
		{"pseudo", (StrataMapStruct){.func = &intt_iterator::pseudo_hash, .map = {}}}
	};

	int progress = 0;
	int PROGRESS = 50;
	std::chrono::high_resolution_clock::time_point TIME = std::chrono::high_resolution_clock::now();
	Long64_t n = 0;

	TFile* i_file = nullptr;
	TTree* i_tree = nullptr;

	TFile* o_file = nullptr;
	TTree* o_tree = nullptr;

	std::string i_file_name = "";
	std::string i_tree_name = "tree";

	std::string o_file_name = "";
	std::string o_tree_name = "";

	RawMap_t raw_counts;

	BranchMap_t::iterator branch_itr;
	HashMap_t::iterator hash_itr;
	RawMap_t::iterator raw_itr;
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

	//////////////////////
	//	Initialization

	std::cout << std::endl;
	std::cout << "Initializing..." << std::endl;

	std::cout << "\tMap:" << std::endl;
	progress = 0;
	for(*intt_itr = intt_itr.begin(); *intt_itr != intt_itr.end(); ++intt_itr)
	{
		raw_counts[intt_itr.get_hash()] = (RawMapStruct){.count = 0, .hashes = 0, .strata_z = {}};

		show_progress(progress, PROGRESS, intt_itr.get_hash(), intt_itr.end() >> 1, TIME);
	}
	std::cout << std::endl;

	std::cout << "\tTree:" << std::endl;
	progress = 0;
	for(n = 0; n < i_tree->GetEntriesFast(); ++n)
	{
		i_tree->GetEntry(n);

		for(branch_itr = branches.begin(); branch_itr != branches.end(); ++branch_itr)
		{
			intt_itr.set_field(branch_itr->second.index, branch_itr->second.count);
		}

		for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
		{
			if(strata_itr->second.map.find((intt_itr.*(strata_itr->second.func))()) == strata_itr->second.map.end())
			{
				strata_itr->second.map[(intt_itr.*(strata_itr->second.func))()] = (HashMapStruct){.count = 0, .hashes = 0};
			}
		}

		if(raw_counts[intt_itr.get_hash()].hashes == 0)
		{
			for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
			{
				++strata_itr->second.map[(intt_itr.*(strata_itr->second.func))()].hashes;
			}

			raw_counts[intt_itr.get_hash()].hashes = 1;
		}

		for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
		{
			++strata_itr->second.map[(intt_itr.*(strata_itr->second.func))()].count;
		}
		++raw_counts[intt_itr.get_hash()].count;

		show_progress(progress, PROGRESS, n, i_tree->GetEntriesFast() - 1, TIME);
	}
	std::cout << std::endl;

	////////////////
	//	Analysis

	std::cout << std::endl;
	std::cout << "Analyzing..." << std::endl;
	for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
	{
		std::cout << "\t" << strata_itr->first << ":" << std::endl;
		progress = 0;

		for(hash_itr = strata_itr->second.map.begin(); hash_itr != strata_itr->second.map.end(); ++hash_itr)
		{
			hash_itr->second.mean = 0;
			hash_itr->second.mean += hash_itr->second.count;
			hash_itr->second.mean /= hash_itr->second.hashes;
			hash_itr->second.sigma = sqrt(hash_itr->second.mean);
		}

		for(raw_itr = raw_counts.begin(); raw_itr != raw_counts.end(); ++raw_itr)
		{
			if(raw_itr->second.count == 0)continue;

			*intt_itr = raw_itr->first;
			*intt_itr = (intt_itr.*(strata_itr->second.func))();

			raw_itr->second.strata_z[strata_itr->first] = raw_itr->second.count;
			raw_itr->second.strata_z[strata_itr->first] -= strata_itr->second.map[*intt_itr].mean;
			raw_itr->second.strata_z[strata_itr->first] /= strata_itr->second.map[*intt_itr].sigma;

			show_progress(progress, PROGRESS, raw_itr->first, intt_itr.end() >> 1, TIME);
		}
		std::cout << std::endl;
	}

	/////////////
	//	Debug

	std::cout << std::endl;
	std::cout << "Debug..." << std::endl;

	std::cout << "\tz scores:" << std::endl;
	for(raw_itr = raw_counts.begin(); raw_itr != raw_counts.end(); ++raw_itr)
	{
		if(raw_itr->second.count == 0)continue;

		for(auto _itr = raw_itr->second.strata_z.begin(); _itr != raw_itr->second.strata_z.end(); ++_itr)
		{
			*intt_itr = raw_itr->first;
			std::cout << "\t" << std::hex << *intt_itr;
			std::cout << "\t(" << std::hex << (intt_itr.*(strata_map[_itr->first].func))() << ")";
			std::cout << "\t" << _itr->first << " z score:";
			std::cout << "\t" << _itr->second;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;

	std::cout << "\tRaw counts:" << std::endl;
	for(auto itr = raw_counts.begin(); itr != raw_counts.end(); ++itr)
	{
		if(itr->second.hashes == 0)continue;

		std::cout << "\t" << std::hex << itr->first;
		std::cout << " :\t" << itr->second.count << std::endl;
	}
	std::cout << std::endl;

	std::cout << "\tCounts by strata:" << std::endl;
	std::cout << std::endl;
	for(strata_itr = strata_map.begin(); strata_itr != strata_map.end(); ++strata_itr)
	{
		std::cout << "\t" << strata_itr->first << std::endl;
		for(auto itr = strata_itr->second.map.begin(); itr != strata_itr->second.map.end(); ++itr)
		{
			if(itr->second.hashes == 0)continue;

			std::cout << "\t" << std::hex << itr->first;
			std::cout << " :\t" << itr->second.count;
			std::cout << " :\t" << itr->second.hashes << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}


//////////////////////////////////////////
//	Global function implementations	//
//////////////////////////////////////////

template <typename T_i, typename T_I, typename T_p, typename T_P>
void show_progress(T_i& _i, T_I const& _I, T_p const& _p, T_P const& _P, std::chrono::high_resolution_clock::time_point& _t)
{
	if(_p * _I < _i * _P)return;
	if(_I < _i)return;

	if(_i == 0)_t = std::chrono::high_resolution_clock::now();

	while(_i * _P < _p * _I)++_i;

	int j = 0;
	std::cout << "\t[";
	while(j < _i && j < _I)
	{
		std::cout << ">";
		++j;
	}
	std::cout << ">";
	++j;
	while(j < _I)
	{
		std::cout << " ";
		++j;
	}

	Double_t d = std::chrono::duration_cast<std::chrono::duration<Double_t>>(std::chrono::high_resolution_clock::now() - _t).count();
	std::cout << "] ";

	if(_i < _I)
	{
		printf("dur: %0.2f s, eta: %0.2f s)", d, d * _I / _i);
		std::cout << "\r";
		std::cout << std::flush;
	}
	else
	{
		printf("(%0.2f s) Done           ", d);
		std::cout << std::endl;
	}
}


/////////////////////
//	For Debugging

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
