/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Edoardo Bossini
 *   Piotr Maciej Cwiklicki
 *   Laurent Forthomme
 *   Tomasz Ostafin
 *
 ****************************************************************************/

#include "DQMServices/Core/interface/DQMEDHarvester.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"

#include "CalibPPS/TimingCalibration/interface/TimingCalibrationStruct.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "CondFormats/PPSObjects/interface/PPSTimingCalibration.h"

#include <Math/MinimizerOptions.h>
#include <TFitResult.h>
//------------------------------------------------------------------------------

class PPSTimingCalibrationPCLHarvester : public DQMEDHarvester {
public:
  PPSTimingCalibrationPCLHarvester(const edm::ParameterSet&);

  void beginRun(const edm::Run&, const edm::EventSetup&) override;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) override;
  bool fetchWorkerHistograms(DQMStore::IGetter&,
                             TimingCalibrationHistograms&,
                             const CTPPSDiamondDetId&,
                             const uint32_t,
                             const std::string&) const;
  std::pair<double, double> findFitRange(const TimingCalibrationHistograms&,
                                         const uint32_t,
                                         const double,
                                         const double) const;

  TF1 interp_;
  const std::string dqmDir_;
  const std::string formula_;
  std::vector<CTPPSDiamondDetId> detIds_;
  const edm::ESGetToken<CTPPSGeometry, VeryForwardRealGeometryRecord> geomEsToken_;
  const unsigned int minEntries_;

  static constexpr int DefaultLowerTotRange_ = 8;
  static constexpr int DefaultUpperTotRange_ = 15;
  static constexpr double UpperLimitMaxSearch_ = 20.0;
  static constexpr double LowerLimitRangeSearch_ = 8.0;
  static constexpr double UpperLimitRangeSearch_ = 20.0;
  static constexpr double FixedFitBound_ = -1.0;
};

//------------------------------------------------------------------------------

PPSTimingCalibrationPCLHarvester::PPSTimingCalibrationPCLHarvester(const edm::ParameterSet& iConfig)
    : interp_{"interp", iConfig.getParameter<std::string>("formula").c_str()},
      dqmDir_{iConfig.getParameter<std::string>("dqmDir")},
      formula_{iConfig.getParameter<std::string>("formula")},
      geomEsToken_{esConsumes<edm::Transition::BeginRun>()},
      minEntries_{iConfig.getParameter<unsigned int>("minEntries")} {
  // first ensure DB output service is available
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  if (!poolDbService.isAvailable())
    throw cms::Exception("PPSTimingCalibrationPCLHarvester") << "PoolDBService required";

  // constrain the min/max fit values
  interp_.SetParLimits(0, 0.5, 5.0);
  interp_.SetParLimits(1, 4.0, 15.0);
  interp_.SetParLimits(2, 0.1, 4.0);
  interp_.SetParLimits(3, 0.1, 15.0);

  // set a higher max function calls limit for the ROOT fit algorithm
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(10'000);
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLHarvester::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
  const auto& geom = iSetup.getData(geomEsToken_);
  for (auto it = geom.beginSensor(); it != geom.endSensor(); ++it) {
    if (CTPPSDiamondDetId::check(it->first)) {
      const CTPPSDiamondDetId detId{it->first};
      detIds_.emplace_back(detId);
    }
  }
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLHarvester::dqmEndJob(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter) {
  // book the parameters containers
  PPSTimingCalibration::ParametersMap calibParams;
  PPSTimingCalibration::TimingMap calibTime;

  iGetter.cd();
  iGetter.setCurrentFolder(dqmDir_);

  constexpr double defaultFitSlope = 0.8;
  constexpr double defaultOffset = 0.0;
  constexpr double defaultResolution = 0.1;
  constexpr std::array<double, 13> thresholds{
      {FixedFitBound_, 0.005, 0.006, 0.007, 0.008, 0.009, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07}};
  // compute the fit parameters for all monitored channels
  TimingCalibrationHistograms hists;
  std::string channelName;
  for (const auto& detId : detIds_) {
    const uint32_t channelId{detId.rawId()};
    detId.channelName(channelName);
    if (fetchWorkerHistograms(iGetter, hists, detId, channelId, channelName)) {
      const PPSTimingCalibration::Key armKey{static_cast<int>(detId.arm()),
                                             static_cast<int>(detId.station()),
                                             static_cast<int>(detId.plane()),
                                             static_cast<int>(detId.channel())};

      MonitorElement* profileHist{iBooker.bookProfile(channelName, channelName, 240, 0.0, 60.0, 450, -20.0, 25.0)};
      std::unique_ptr<TProfile> tProfile{hists.leadingTimeVsToT[channelId]->getTH2F()->ProfileX()};
      *(profileHist->getTProfile()) = *(static_cast<TProfile*>(tProfile->Clone()));
      const char* profileName = channelName.c_str();
      profileHist->getTProfile()->SetTitle(profileName);
      profileHist->getTProfile()->SetName(profileName);
      profileHist->getTProfile()->SetYTitle("Average t (ns)");

      const double defaultUpperLowerAsymptotesDiff = hists.leadingTime[channelId]->getRMS();
      const double defaultCenterOfDistribution = hists.toT[channelId]->getMean();
      const double defaultLowerAsymptote = hists.leadingTime[channelId]->getMean() - defaultUpperLowerAsymptotesDiff;

      double bestChiSqDivNdf{std::numeric_limits<double>::max()};
      double bestLowerTotRange{0.0};
      double bestUpperTotRange{0.0};
      for (const double upperThresholdFractionOfMax : thresholds) {
        for (const double lowerThresholdFractionOfMax : thresholds) {
          interp_.SetParameters(
              defaultUpperLowerAsymptotesDiff, defaultCenterOfDistribution, defaultFitSlope, defaultLowerAsymptote);
          const auto [lowerTotRange, upperTotRange] =
              findFitRange(hists, channelId, lowerThresholdFractionOfMax, upperThresholdFractionOfMax);

          const TFitResultPtr& fitResult{
              profileHist->getTProfile()->Fit(&interp_, "BNS", "", lowerTotRange, upperTotRange)};
          if (fitResult->IsValid()) {
            const double chiSqDivNdf{fitResult->Chi2() / fitResult->Ndf()};
            if (chiSqDivNdf < bestChiSqDivNdf) {
              bestChiSqDivNdf = chiSqDivNdf;
              bestUpperTotRange = upperTotRange;
              bestLowerTotRange = lowerTotRange;
            }
          }
        }
      }

      calibParams[armKey] = {0, 0, 0, 0};
      calibTime[armKey] = {defaultOffset, defaultResolution};
      if (bestLowerTotRange != 0.0) {
        profileHist->getTProfile()->Fit(&interp_, "B", "", bestLowerTotRange, bestUpperTotRange);
        calibParams[armKey] = {
            interp_.GetParameter(0), interp_.GetParameter(1), interp_.GetParameter(2), interp_.GetParameter(3)};
        calibTime[armKey] = {defaultOffset,
                             defaultResolution};  // hardcoded offset/resolution placeholder for the time being
      } else {
        edm::LogWarning("PPSTimingCalibrationPCLHarvester:dqmEndJob")
            << "Fit did not converge for channel (" << detId << ").";
      }
    }
  }

  // fill the DB object record
  PPSTimingCalibration calib(formula_, calibParams, calibTime);

  // write the object
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  poolDbService->writeOneIOV(calib, poolDbService->currentTime(), "PPSTimingCalibrationRcd_HPTDC");
}

//------------------------------------------------------------------------------

bool PPSTimingCalibrationPCLHarvester::fetchWorkerHistograms(DQMStore::IGetter& iGetter,
                                                             TimingCalibrationHistograms& hists,
                                                             const CTPPSDiamondDetId& detId,
                                                             const uint32_t channelId,
                                                             const std::string& channelName) const {
  hists.leadingTime[channelId] = iGetter.get(dqmDir_ + "/t_" + channelName);
  if (!hists.leadingTime[channelId]) {
    edm::LogWarning("PPSTimingCalibrationPCLHarvester:fetchWorkerHistograms")
        << "Failed to retrieve leading time monitor for channel (" << detId << "). Skipping calibration.";
    return false;
  }

  hists.toT[channelId] = iGetter.get(dqmDir_ + "/tot_" + channelName);
  if (!hists.toT[channelId]) {
    edm::LogWarning("PPSTimingCalibrationPCLHarvester:fetchWorkerHistograms")
        << "Failed to retrieve time over threshold monitor for channel (" << detId << "). Skipping calibration.";
    return false;
  }

  hists.leadingTimeVsToT[channelId] = iGetter.get(dqmDir_ + "/tvstot_" + channelName);
  if (!hists.leadingTimeVsToT[channelId]) {
    edm::LogWarning("PPSTimingCalibrationPCLHarvester:fetchWorkerHistograms")
        << "Failed to retrieve leading time vs. time over threshold monitor for channel (" << detId
        << "). Skipping calibration.";
    return false;
  }

  if (hists.leadingTimeVsToT[channelId]->getEntries() < minEntries_) {
    edm::LogWarning("PPSTimingCalibrationPCLHarvester:fetchWorkerHistograms")
        << "Not enough entries for channel (" << detId << "): " << hists.leadingTimeVsToT[channelId]->getEntries()
        << " < " << minEntries_ << ". Skipping calibration.";
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------

std::pair<double, double> PPSTimingCalibrationPCLHarvester::findFitRange(
    const TimingCalibrationHistograms& hists,
    const uint32_t channelId,
    const double lowerThresholdFractionOfMax,
    const double upperThresholdFractionOfMax) const {
  int maxTotBin{1};
  const int numOfToTBins{hists.toT.at(channelId)->getNbinsX()};
  for (int i{2}; i <= numOfToTBins; ++i) {
    if (hists.toT.at(channelId)->getTH1()->GetXaxis()->GetBinCenter(i) > UpperLimitMaxSearch_) {
      break;
    }
    if (hists.toT.at(channelId)->getBinContent(i) > hists.toT.at(channelId)->getBinContent(maxTotBin)) {
      maxTotBin = i;
    }
  }

  double lowerTotRange{DefaultLowerTotRange_};
  if (lowerThresholdFractionOfMax != FixedFitBound_) {
    int lowerLimitPos{maxTotBin};
    const double lower_threshold{lowerThresholdFractionOfMax * hists.toT.at(channelId)->getBinContent(maxTotBin)};
    while (hists.toT.at(channelId)->getTH1()->GetXaxis()->GetBinCenter(lowerLimitPos) > LowerLimitRangeSearch_) {
      --lowerLimitPos;
      if (hists.toT.at(channelId)->getBinContent(lowerLimitPos) < lower_threshold)
        break;
    }
    lowerTotRange = hists.toT.at(channelId)->getTH1()->GetXaxis()->GetBinCenter(lowerLimitPos);
  }

  double upperTotRange{DefaultUpperTotRange_};
  if (upperThresholdFractionOfMax != FixedFitBound_) {
    int upperLimitPos{maxTotBin};
    const double upper_threshold{upperThresholdFractionOfMax * hists.toT.at(channelId)->getBinContent(maxTotBin)};
    while (hists.toT.at(channelId)->getTH1()->GetXaxis()->GetBinCenter(upperLimitPos) < UpperLimitRangeSearch_) {
      ++upperLimitPos;
      if (hists.toT.at(channelId)->getBinContent(upperLimitPos) < upper_threshold)
        break;
    }
    upperTotRange = hists.toT.at(channelId)->getTH1()->GetXaxis()->GetBinCenter(upperLimitPos);
  }

  return {lowerTotRange, upperTotRange};
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLHarvester::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("dqmDir", "AlCaReco/PPSTimingCalibrationPCL")
      ->setComment("input path for the various DQM plots");
  desc.add<std::string>("formula", "[0]/(exp((x-[1])/[2])+1)+[3]")
      ->setComment("interpolation formula for the time walk component");
  desc.add<unsigned int>("minEntries", 100)->setComment("minimal number of hits to extract calibration");
  descriptions.addWithDefaultLabel(desc);
}

DEFINE_FWK_MODULE(PPSTimingCalibrationPCLHarvester);
