#include "DiffuseLaserEventSelector.h"

#include <tpc/LaserEventInfo.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
#include <phool/phool.h>

#include <iostream>

DiffuseLaserEventSelector::DiffuseLaserEventSelector(const std::string& name)
  : SubsysReco(name)
{
}

int DiffuseLaserEventSelector::process_event(PHCompositeNode* topNode)
{
  LaserEventInfo* laserEventInfo =
      findNode::getClass<LaserEventInfo>(topNode, "LaserEventInfo");

  if (!laserEventInfo)
  {
    std::cout << PHWHERE
              << " LaserEventInfo node is missing. Rejecting event."
              << std::endl;

    return Fun4AllReturnCodes::DISCARDEVENT;
  }

  bool accept = true;

  if (m_requireTPCDiffuseLaser)
  {
    accept = accept && laserEventInfo->isLaserEvent();
  }

  if (m_requireGL1Laser)
  {
    accept = accept && laserEventInfo->isGl1LaserEvent();
  }

  if (m_rejectGL1Pileup)
  {
    accept = accept && !laserEventInfo->isGl1LaserPileupEvent();
  }

  if (Verbosity() > 1)
  {
    std::cout << "DiffuseLaserEventSelector:"
              << " isLaserEvent = " << laserEventInfo->isLaserEvent()
              << " isGl1LaserEvent = " << laserEventInfo->isGl1LaserEvent()
              << " isGl1LaserPileupEvent = "
              << laserEventInfo->isGl1LaserPileupEvent()
              << " accept = " << accept
              << std::endl;
  }

  if (accept)
  {
    return Fun4AllReturnCodes::EVENT_OK;
  }

  return Fun4AllReturnCodes::DISCARDEVENT;
}