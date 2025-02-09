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

/// @file   TRDGlobalTrackingSpec.h

#ifndef O2_TRD_GLOBALTRACKING
#define O2_TRD_GLOBALTRACKING

#include "Framework/DataProcessorSpec.h"
#include "Framework/Task.h"
#include "TStopwatch.h"
#include "TRDBase/GeometryFlat.h"
#include "GPUO2Interface.h"
#include "GPUTRDTracker.h"
#include "ReconstructionDataFormats/GlobalTrackID.h"
#include "DataFormatsGlobalTracking/RecoContainer.h"
#include "DataFormatsTRD/TrackTRD.h"
#include "DataFormatsTRD/TrackTriggerRecord.h"
#include <memory>

namespace o2
{
namespace trd
{

class TRDGlobalTracking : public o2::framework::Task
{
 public:
  TRDGlobalTracking(bool useMC, std::shared_ptr<o2::globaltracking::DataRequest> dataRequest, o2::dataformats::GlobalTrackID::mask_t src, bool trigRecFilterActive, bool strict) : mUseMC(useMC), mDataRequest(dataRequest), mTrkMask(src), mTrigRecFilter(trigRecFilterActive), mStrict(strict) {}
  ~TRDGlobalTracking() override = default;
  void init(o2::framework::InitContext& ic) final;
  void updateTimeDependentParams();
  void fillTrackTriggerRecord(const std::vector<TrackTRD>& tracks, std::vector<TrackTriggerRecord>& trigRec, const gsl::span<const o2::trd::TriggerRecord>& trackletTrigRec) const;
  void run(o2::framework::ProcessingContext& pc) final;
  void endOfStream(o2::framework::EndOfStreamContext& ec) final;

 private:
  o2::gpu::GPUTRDTracker* mTracker{nullptr};          ///< TRD tracking engine
  o2::gpu::GPUReconstruction* mRec{nullptr};          ///< GPU reconstruction pointer, handles memory for the tracker
  o2::gpu::GPUChainTracking* mChainTracking{nullptr}; ///< TRD tracker is run in the tracking chain
  std::unique_ptr<GeometryFlat> mFlatGeo{nullptr};    ///< flat TRD geometry
  bool mUseMC{false};                                 ///< MC flag
  bool mTrigRecFilter{false};                         ///< if true, TRD trigger records without matching ITS IR are filtered out
  bool mStrict{false};                                ///< preliminary matching in strict mode
  float mTPCTBinMUS{.2f};                             ///< width of a TPC time bin in us
  float mTPCVdrift{2.58f};                            ///< TPC drift velocity (for shifting TPC tracks along Z)
  std::shared_ptr<o2::globaltracking::DataRequest> mDataRequest; ///< seeding input (TPC-only, ITS-TPC or both)
  o2::dataformats::GlobalTrackID::mask_t mTrkMask;               ///< seeding track sources (TPC, ITS-TPC)
  TStopwatch mTimer;
};

/// create a processor spec
framework::DataProcessorSpec getTRDGlobalTrackingSpec(bool useMC, o2::dataformats::GlobalTrackID::mask_t src, bool trigRecFilterActive, bool strict = false);

} // namespace trd
} // namespace o2

#endif /* O2_TRD_TRACKLETREADER */
