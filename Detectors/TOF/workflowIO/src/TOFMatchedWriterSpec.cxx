// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// @file   TOFMatchedWriterSpec.cxx

#include "TOFWorkflowIO/TOFMatchedWriterSpec.h"
#include "Framework/ControlService.h"
#include "Framework/ConfigParamRegistry.h"
#include "DPLUtils/MakeRootTreeWriterSpec.h"
#include "Headers/DataHeader.h"
#include <SimulationDataFormat/MCCompLabel.h>
#include <SimulationDataFormat/MCTruthContainer.h>
#include "ReconstructionDataFormats/MatchInfoTOF.h"
#include "ReconstructionDataFormats/MatchingType.h"
#include "ReconstructionDataFormats/TrackTPCTOF.h"
#include "ReconstructionDataFormats/GlobalTrackID.h"
#include "DataFormatsTOF/CalibInfoTOF.h"
#include "DataFormatsTOF/Cluster.h"
#include "CommonUtils/StringUtils.h"

using namespace o2::framework;

namespace o2
{
namespace tof
{

template <typename T>
using BranchDefinition = MakeRootTreeWriterSpec::BranchDefinition<T>;
using MatchInfo = std::vector<o2::dataformats::MatchInfoTOF>;
using TrackInfo = std::vector<o2::dataformats::TrackTPCTOF>;
using LabelsType = std::vector<o2::MCCompLabel>;
using namespace o2::header;

DataProcessorSpec getTOFMatchedWriterSpec(bool useMC, const char* outdef, bool writeTOFTPC, bool strictMode)
{
  // spectators for logging
  auto loggerMatched = [](MatchInfo const& indata) {
    LOG(INFO) << "RECEIVED MATCHED SIZE " << indata.size();
  };
  auto loggerTofLabels = [](LabelsType const& labeltof) {
    LOG(INFO) << "TOF LABELS GOT " << labeltof.size() << " LABELS ";
  };
  o2::header::DataDescription ddMatchInfo{"MTC_ITSTPC"}, ddMatchInfo_tpc{"MTC_TPC"},
    ddMCMatchTOF{"MCMTC_ITSTPC"}, ddMCMatchTOF_tpc{"MCMTC_TPC"};
  uint32_t ss = o2::globaltracking::getSubSpec(strictMode && writeTOFTPC ? o2::globaltracking::MatchingType::Strict : o2::globaltracking::MatchingType::Standard);
  return MakeRootTreeWriterSpec(writeTOFTPC ? "TOFMatchedWriter_TPC" : "TOFMatchedWriter",
                                outdef,
                                "matchTOF",
                                BranchDefinition<MatchInfo>{InputSpec{"tofmatching", gDataOriginTOF, writeTOFTPC ? ddMatchInfo_tpc : ddMatchInfo, ss},
                                                            "TOFMatchInfo",
                                                            "TOFMatchInfo-branch-name",
                                                            1,
                                                            loggerMatched},
                                BranchDefinition<TrackInfo>{InputSpec{"tpctofTracks", gDataOriginTOF, "TOFTRACKS_TPC", ss},
                                                            "TPCTOFTracks",
                                                            "TPCTOFTracks-branch-name",
                                                            writeTOFTPC},
                                BranchDefinition<LabelsType>{InputSpec{"matchtoflabels", gDataOriginTOF, writeTOFTPC ? ddMCMatchTOF_tpc : ddMCMatchTOF, ss},
                                                             "MatchTOFMCTruth",
                                                             "MatchTOFMCTruth-branch-name",
                                                             (useMC ? 1 : 0), // one branch if mc labels enabled
                                                             loggerTofLabels})();
}
} // namespace tof
} // namespace o2
