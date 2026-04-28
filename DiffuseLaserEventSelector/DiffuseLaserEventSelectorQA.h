#ifndef DiffuseLaserEventSelectorQA_H
#define DiffuseLaserEventSelectorQA_H

#include <fun4all/SubsysReco.h>

#include <string>

class PHCompositeNode;
class TrkrHitSetContainer;
class TFile;
class TTree;

class DiffuseLaserEventSelectorQA : public SubsysReco
{
 public:
  DiffuseLaserEventSelectorQA(const std::string& name = "DiffuseLaserEventSelectorQA",
                              const std::string &filename = "DiffuseLaserEventSelectorQA.root");

  ~DiffuseLaserEventSelectorQA();

  int Init(PHCompositeNode *topNode) override;
  int InitRun(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode *topNode) override;
  int End(PHCompositeNode *topNode) override;




 private:
  bool m_isLaserEvent = true;
  unsigned int m_layer = 999;
  unsigned int m_sector = 999;
  unsigned int m_side = 999;
  unsigned int m_pad = 999;
   int m_timebin = 999;
  unsigned int m_adc = 0;
  unsigned int n_event = 0;
  
  float m_hit_x = 0;
  float m_hit_y = 0;
  float m_hit_z = 0;
  

  uint32_t m_hitsetkey = std::numeric_limits<uint32_t>::quiet_NaN();
   TrkrHitSetContainer *hitmap = nullptr;

  std::string m_outputFileName;
  TFile *m_outputFile;
  TTree *m_tree;

};

#endif