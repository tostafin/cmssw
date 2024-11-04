// #include "CalibPPS/TimingCalibration/interface/DoublePeakCorrection.h"
#include "../interface/DoublePeakCorrection.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "TFitResult.h"

void DoublePeakCorrection::extractLsAndTimeOffset(const std::string& tVsLsFilename,
                                                  const unsigned int run,
                                                  const std::vector<CTPPSDiamondDetId>& detIds) {
  if (!tVsLsFilename.empty()) {
    TFile tVsLsFile{tVsLsFilename.c_str()};
    if (tVsLsFile.IsOpen()) {
      const std::string runNumber{std::to_string(run)};
      for (const auto& detId : detIds) {
        const PlaneKey planeKey{detId.arm(), detId.station(), detId.plane()};
        if (lsAndTimeOffset_.count(planeKey) == 0) {
          const TH2F* tVsLs{getTVsLs(tVsLsFile, runNumber, detId)};
          fillLsAndTimeOffset(tVsLs, planeKey);
        }
      }
    } else {
      throw edm::Exception{edm::errors::FileOpenError} << "Can't open the file with t vs LS: " << tVsLsFilename << '.';
    }
  }
}

const TH2F* DoublePeakCorrection::getTVsLs(TFile& tVsLsFile,
                                           const std::string& runNumber,
                                           const CTPPSDiamondDetId& detId) {
  std::string planeName;
  detId.planeName(planeName);
  std::string tVsLsHistPath{"DQMData/Run " + runNumber + "/AlCaReco/Run summary/PPSTimingCalibrationPCL/tvsls_" +
                            planeName};
  const auto* tVsLs = tVsLsFile.Get<TH2F>(tVsLsHistPath.c_str());
  if (tVsLs) {
    return tVsLs;
  }
  throw edm::Exception{edm::errors::FileReadError} << "Can't open the t vs LS histogram: " << tVsLsHistPath << '.';
}

void DoublePeakCorrection::fillLsAndTimeOffset(const TH2F* tVsLs, const PlaneKey& planeKey) {
  unsigned int doublePeakLs{1};
  auto numOfLs = static_cast<unsigned int>(tVsLs->GetNbinsX());
  auto numOfTBins = static_cast<unsigned int>(tVsLs->GetNbinsX());
  double firstPeakTWithMaxCount{0.0};
  double secondPeakTWithMaxCount{0.0};
  double prevTWithMaxCount{0.0};
  uint8_t doublePeakOccurrences{0};
  constexpr uint8_t requiredDoublePeakOccurrences{2};
  for (unsigned int lsBin{2}; lsBin <= numOfLs; ++lsBin) {
    double tMaxCount{0};
    for (unsigned int tBin{1}; tBin <= numOfTBins; ++tBin) {
      const double tCount{tVsLs->GetBinContent(lsBin, tBin)};
      if (tCount > tMaxCount) {
        tMaxCount = tCount;
        secondPeakTWithMaxCount = tVsLs->GetYaxis()->GetBinCenter(tBin);
      }
    }

    if (tMaxCount != 0.0 && prevTWithMaxCount != 0.0) {
      const double timeDiff{secondPeakTWithMaxCount - prevTWithMaxCount};
      if (timeDiff > TMaxDiff_) {
        if (doublePeakOccurrences == 0) {
          doublePeakLs = lsBin;
        }
        ++doublePeakOccurrences;
        if (doublePeakOccurrences == requiredDoublePeakOccurrences) {
          break;
        }
      }
    }

    if (doublePeakOccurrences == 0) {
      firstPeakTWithMaxCount = prevTWithMaxCount;
      prevTWithMaxCount = secondPeakTWithMaxCount;
    }
  }

  if (doublePeakLs != 1) {
    lsAndTimeOffset_[planeKey] = {doublePeakLs, findTimeOffset(tVsLs, firstPeakTWithMaxCount, secondPeakTWithMaxCount)};
  }
}

double DoublePeakCorrection::findTimeOffset(const TH2F* tVsLs,
                                            const double firstPeakEstimatedMean,
                                            const double secondPeakEstimatedMean) {
  std::unique_ptr<TH1D> tProjection{tVsLs->ProjectionY()};
  return findGaussianMean(tProjection, secondPeakEstimatedMean) - findGaussianMean(tProjection, firstPeakEstimatedMean);
}

double DoublePeakCorrection::findGaussianMean(const std::unique_ptr<TH1D>& tProjection, const double estimatedMean) {
  const TFitResultPtr& peakFit{
      tProjection->Fit("gaus", "NS", "", estimatedMean - TMaxDiff_, estimatedMean + TMaxDiff_)};
  if (peakFit->IsValid()) {
    return peakFit->Parameter(1);
  }
  return estimatedMean;
}

double DoublePeakCorrection::getCorrectedLeadingTime(const double leadingTime,
                                                     const unsigned int ls,
                                                     const PlaneKey& planeKey) const {
  if (auto it = lsAndTimeOffset_.find(planeKey); it != std::end(lsAndTimeOffset_)) {
    const auto [doublePeakLs, doublePeakTimeOffset] = it->second;
    if (ls >= doublePeakLs) {
      return leadingTime - doublePeakTimeOffset;
    }
  }
  return leadingTime;
}

double DoublePeakCorrection::getEncodedLsAndTimeOffset(const PlaneKey& planeKey) const {
  if (auto it = lsAndTimeOffset_.find(planeKey); it != std::end(lsAndTimeOffset_)) {
    constexpr double encodingMultiple = 100'000.0;
    const auto [doublePeakLs, doublePeakTimeOffset] = it->second;
    return doublePeakLs * encodingMultiple + doublePeakTimeOffset;
  }
  return 0.0;
}
