#include <iostream>
#include <string>
#include <map>

#include "TFile.h"
#include "TTree.h"

int main()
{
	std::map<std::string, Long64_t> branches =
	{
		{"lyr", 0},
		{"ldr", 0},
		{"nos", 0},
		{"chp", 0},
		{"chn", 0}
	};

	TFile* foo = TFile::Open("foo.root", "recreate");
	TTree* tree = new TTree("tree", "tree");
	tree->SetDirectory(foo);

	for(auto itr = branches.begin(); itr != branches.end(); ++itr)
	{
		tree->Branch((itr->first).c_str(), &(itr->second));
	}

	branches["lyr"] = 1;
	branches["ldr"] = 9;
	branches["nos"] = 1;
	branches["chp"] = 13;
	branches["chn"] = 96;
	tree->Fill();

	branches["lyr"] = 1;
	branches["ldr"] = 9;
	branches["nos"] = 1;
	branches["chp"] = 13;
	branches["chn"] = 96;
	tree->Fill();

	branches["lyr"] = 1;
	branches["ldr"] = 8;
	branches["nos"] = 1;
	branches["chp"] = 13;
	branches["chn"] = 3;
	tree->Fill();

	branches["lyr"] = 1;
	branches["ldr"] = 7;
	branches["nos"] = 1;
	branches["chp"] = 0;
	branches["chn"] = 127;
	tree->Fill();

	branches["lyr"] = 2;
	branches["ldr"] = 9;
	branches["nos"] = 1;
	branches["chp"] = 13;
	branches["chn"] = 96;
	tree->Fill();

	tree->Write();
	foo->Write();
	foo->Close();

	return 0;
}
