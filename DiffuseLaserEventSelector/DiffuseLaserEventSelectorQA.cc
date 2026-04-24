#include "DiffuseLaserEventSelectorQA.h"

#include <tpc/LaserEventInfo.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
#include <phool/phool.h>


#include <trackbase/TrkrDefs.h>
#include <trackbase/TpcDefs.h>
#include <trackbase/TrkrHit.h>
#include <trackbase/TrkrHitSet.h>
#include <trackbase/TrkrHitSetContainer.h>
#include <trackbase/TrkrClusterHitAssocv3.h>

#include <TFile.h>
#include <TTree.h>

#include <iostream>

DiffuseLaserEventSelectorQA::DiffuseLaserEventSelectorQA(const std::string& name, const std::string &filename)
  : SubsysReco(name)
  , m_outputFileName(filename)
  , m_outputFile(nullptr)
  , m_tree(nullptr)
{
}
//--------------------------------------------------------------------------------
DiffuseLaserEventSelectorQA::~DiffuseLaserEventSelectorQA()
{
  if (m_outputFile)
  {
    m_outputFile->Close();
    delete m_outputFile;
  }
}
//--------------------------------------------------------------------------------
int DiffuseLaserEventSelectorQA::Init(PHCompositeNode* /*topNode*/)
{
  std::cout << "DiffuseLaserEventSelectorQA::Init " << m_outputFileName << std::endl;
  if (m_outputFileName!="")
  {
      m_outputFile = new TFile(m_outputFileName.c_str(), "RECREATE");
      if (!m_outputFile || m_outputFile->IsZombie())
      {
        std::cout << "DiffuseLaserEventSelectorQA::Init - Error: Cannot create output root file" << std::endl;
        return Fun4AllReturnCodes::ABORTRUN;
      }

      m_tree = new TTree("DiffuseLaserEventSelectorQA", "Diffuse laser events");
      m_tree->Branch("isLaserEvent", &m_isLaserEvent, "m_isLaserEvent/O");
      m_tree->Branch("layer", &m_layer, "m_layer/I");
      m_tree->Branch("sector", &m_sector, "m_sector/I");
      m_tree->Branch("side", &m_side, "m_side/I");
      m_tree->Branch("pad", &m_pad, "m_pad/I");
      m_tree->Branch("timebin", &m_timebin, "m_timebin/I");
      m_tree->Branch("adc", &m_adc, "m_adc/I");

  }

  //std::cout << "DiffuseLaserEventSelectorQA::Init - Initialization complete" << std::endl;

  return Fun4AllReturnCodes::EVENT_OK;
}

//--------------------------------------------------------------------------------
int DiffuseLaserEventSelectorQA::InitRun(PHCompositeNode *topNode)
{
  hitmap = findNode::getClass<TrkrHitSetContainer>(topNode, "TRKR_HITSET");
  if (!hitmap)
  {
    std::cout << PHWHERE << "ERROR: Can't find node TRKR_HITSET" << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

//--------------------------------------------------------------------------------

int DiffuseLaserEventSelectorQA::process_event(PHCompositeNode* /*topNode*/)
{

  TrkrHitSetContainer::ConstRange all_hitsets = hitmap->getHitSets();
  for (TrkrHitSetContainer::ConstIterator hitsetiter = all_hitsets.first;
       hitsetiter != all_hitsets.second;
       ++hitsetiter)
  {
    m_hitsetkey = hitsetiter->first;
    TrkrHitSet* hitset = hitsetiter->second;

    m_layer = TrkrDefs::getLayer(m_hitsetkey);
    auto det = TrkrDefs::getTrkrId(m_hitsetkey);

    if(det!=TrkrDefs::TrkrId::tpcId) continue;
    m_sector = TpcDefs::getSectorId(m_hitsetkey);
    m_side = TpcDefs::getSide(m_hitsetkey);
  

  auto hitrangei = hitset->getHits();
    for (TrkrHitSet::ConstIterator hitr = hitrangei.first;
         hitr != hitrangei.second;
         ++hitr)
    {
      auto hitkey = hitr->first;
      auto *hit = hitr->second;
      m_adc = hit->getAdc();
      if(det!=TrkrDefs::TrkrId::tpcId) continue;
      m_pad = TpcDefs::getPad(hitkey);
      m_timebin = TpcDefs::getTBin(hitkey);

      m_tree->Fill();
    }

    
  }
  return Fun4AllReturnCodes::EVENT_OK;
}
//--------------------------------------------------------------------------------
int DiffuseLaserEventSelectorQA::End(PHCompositeNode* /*topNode*/)
{
  if (m_outputFile)
  {
    m_outputFile->cd();
    m_tree->Write();
    m_outputFile->Close();
  }

  return Fun4AllReturnCodes::EVENT_OK;
}