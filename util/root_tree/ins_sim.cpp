// ins_sim.cpp
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TCollection.h"  // for TIter
#include <iostream>
#include <cstring>

// Minimal test: just opens the file and prints the tree list.
// We’ll add plots once this runs successfully.

void ins_sim(const char* filename = "../../worksim/p_kin1.root")
{
    std::cout << "[ins_sim] Opening file: " << filename << std::endl;

    TFile* f = TFile::Open(filename);
    if (!f || f->IsZombie()) {
        std::cerr << "[ins_sim] ERROR: Could not open file." << std::endl;
        return;
    }

    std::cout << "[ins_sim] File contents:" << std::endl;
    f->ls();

    // Find the first TTree
    TTree* T = nullptr;
    TIter nextkey(f->GetListOfKeys());
    while (TKey* key = (TKey*)nextkey()) {
        if (std::strcmp(key->GetClassName(), "TTree") == 0) {
            T = (TTree*)key->ReadObj();
            std::cout << "[ins_sim] Using TTree: " << key->GetName() << std::endl;
            break;
        }
    }

    if (!T) {
        std::cerr << "[ins_sim] ERROR: No TTree found in file!" << std::endl;
        return;
    }

    std::cout << "\n[ins_sim] TTree structure:\n";
    T->Print();

    std::cout << "\n[ins_sim] First event (entry 0):\n";
    T->Show(0);

    // Example simple histogram of some branch — replace "hsdelta"
    if (!T->GetBranch("hsdelta")) {
        std::cout << "[ins_sim] Branch 'hsdelta' not found; skipping plot.\n";
        return;
    }

    TCanvas* c1 = new TCanvas("c1", "hsdelta", 800, 600);
    T->Draw("hsdelta >> h_hsdelta(100)", "", "E");
    c1->SetGrid();
}
