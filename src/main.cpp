#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "TFile.h"
#include "TTree.h"

#include "intt_hash.hpp"

int main(int argc, char** argv)
{
	//variable declarations
	std::string tree_name = "tree";
	std::map<std::string, std::string> m =
	{
		{"branch1",		"branch1"},
		{"branch2",		"branch2"}
	};

	TFile* i_file = nullptr;
	TFile* o_file = nullptr;

	TTree* i_tree = nullptr;
	TTree* o_tree = nullptr;

	//brunt of the function
	std::vector<std::string> file_names;
	for(int a = 1; a < argc; ++a)
		file_names.push_back(std::string(argv[a]));

	for(auto itr = file_names.begin(); itr != file_names.end(); ++itr)
	{
		std::cout << *itr << std::endl;

		i_file = nullptr;
		i_file = TFile::Open((*itr).c_str());
		if(!i_file)
		{
			std::cout << "Could not open file:" << std::endl;
			std::cout << "\t" << i_file << std::endl;
			continue;
		}

		i_tree = nullptr;
		i_tree = (TTree*)i_file->FindObject(tree_name.c_str());
	}

	//output and cleanup
	o_tree->Write();
	o_file->Write();
	o_file->Close();

	return 0;
}
