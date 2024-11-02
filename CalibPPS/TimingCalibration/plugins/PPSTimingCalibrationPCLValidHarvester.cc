/****************************************************************************
 *
 * This is a part of PPS offline software.
 * Authors:
 *   Edoardo Bossini
 *   Piotr Maciej Cwiklicki
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include <Math/MinimizerOptions.h>
#include <TFile.h>
#include "DQMServices/Core/interface/DQMEDHarvester.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/CTPPSGeometry.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"

#include "CalibPPS/TimingCalibration/interface/TimingCalibrationStruct.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "CondFormats/PPSObjects/interface/PPSTimingCalibration.h"
#include "TFitResult.h"

using PlaneKey = std::tuple<uint8_t, uint8_t, uint8_t>;

//------------------------------------------------------------------------------

class PPSTimingCalibrationPCLValidHarvester : public DQMEDHarvester {
public:
  PPSTimingCalibrationPCLValidHarvester(const edm::ParameterSet&);
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  // void dqmAnalyze(DQMStore::IBooker&, DQMStore::IGetter&, const edm::Event&, const edm::EventSetup&) override;

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void extractDoublePeakLsAndOffset(const PlaneKey& key, const TH2F* tVsLs);
  void dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) override;
  edm::ESGetToken<CTPPSGeometry, VeryForwardRealGeometryRecord> geomEsToken_;
  std::vector<CTPPSDiamondDetId> detids_;
  const std::string dqmDir_;
  const std::string formula_;
  static constexpr std::array<double, 13> thresholds_{{
    -1.0,  // fixed bounds
    0.005,
    0.006,
    0.007,
    0.008,
    0.009,
    0.01,
    0.02,
    0.03,
    0.04,
    0.05,
    0.06,
    0.07,
  }};
  const unsigned int min_entries_;
  static constexpr double upper_limit_max_search_ = 20;
  static constexpr double upper_limit_range_search_ = 20;
  static constexpr double lower_limit_range_search_ = 8;
  static constexpr double resolution_ = 0.1;
  static constexpr double offset_ = 0.;
  TF1 interp_;
  const std::string tVsLsFileName_;
  static constexpr double double_peak_t_max_diff_ = 3.;
  std::unordered_map<PlaneKey, std::pair<unsigned int, double>> doublePeakLsAndOffset;
};

//------------------------------------------------------------------------------

PPSTimingCalibrationPCLValidHarvester::PPSTimingCalibrationPCLValidHarvester(const edm::ParameterSet& iConfig)
    : geomEsToken_(esConsumes<edm::Transition::BeginRun>()),
      dqmDir_(iConfig.getParameter<std::string>("dqmDir")),
      formula_(iConfig.getParameter<std::string>("formula")),
      min_entries_(iConfig.getParameter<unsigned int>("minEntries")),
      interp_("interp", formula_.c_str(), 10.5, 25.),
      tVsLsFileName_(iConfig.getParameter<std::string>("tVsLsFileName")) {
  // first ensure DB output service is available
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  if (!poolDbService.isAvailable())
    throw cms::Exception("PPSTimingCalibrationPCLValidHarvester") << "PoolDBService required";

  // constrain the min/max fit values
  interp_.SetParLimits(0, 0.5, 5.);
  interp_.SetParLimits(1, 4., 15.);
  interp_.SetParLimits(2, 0.1, 4.);
  interp_.SetParLimits(3, 0.1, 15.);

  // set higher max function calls limit
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(10'000);
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidHarvester::extractDoublePeakLsAndOffset(const PlaneKey& key, const TH2F* tVsLs) {
  unsigned int double_peak_ls = 1;
  unsigned int n_bins_x = tVsLs->GetNbinsX();
  unsigned int n_bins_y = tVsLs->GetNbinsX();
  double prev_t_with_max_count = -1.;
  double time_shift = 0.;
  for (unsigned int bin_x = 1; bin_x <= n_bins_x; ++bin_x) {
    double t_with_max_count = 1;
    double max_t_count = 0;
    for (unsigned int bin_y = 1; bin_y <= n_bins_y; ++bin_y) {
      double t_count = tVsLs->GetBinContent(bin_x, bin_y);
      if (t_count > max_t_count) {
        max_t_count = t_count;
        t_with_max_count = tVsLs->GetYaxis()->GetBinCenter(bin_y);
      }
    }
    std::cout << "Max count for LS = " << bin_x << " and t = " << t_with_max_count << ": " << max_t_count << ". Prev max t: " << prev_t_with_max_count << '\n';
    if (max_t_count != 0.) {
      double time_diff = t_with_max_count - prev_t_with_max_count;
      if (prev_t_with_max_count != -1. && abs(time_diff) > double_peak_t_max_diff_) {
        double_peak_ls = bin_x;
        time_shift = time_diff;
        break;
      }
      prev_t_with_max_count = t_with_max_count;
    }
  }
  doublePeakLsAndOffset[key] = {double_peak_ls, time_shift};
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidHarvester::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
  auto* efficiencyFile = new TFile(tVsLsFileName_.data(), "READ");
  if (!efficiencyFile->IsOpen()) {
    throw edm::Exception(edm::errors::FileOpenError) << "Can't open tVsLs file: " << tVsLsFileName_;
  }
  const auto& geom = iSetup.getData(geomEsToken_);
  for (auto it = geom.beginSensor(); it != geom.endSensor(); ++it) {
    if (!CTPPSDiamondDetId::check(it->first))
      continue;
    const CTPPSDiamondDetId detid(it->first);
    detids_.emplace_back(detid);
    PlaneKey key{detid.arm(), detid.station(), detid.plane()};
    std::string plane_name;
    detid.planeName(plane_name);
    std::string namecycle = "DQMData/Run " + std::to_string(iRun.run()) + "/AlCaReco/Run summary/PPSTimingCalibrationPCL/tvsls_" + plane_name;
    auto* tVsLs = (TH2F*) efficiencyFile->Get(namecycle.data());
    extractDoublePeakLsAndOffset(key, tVsLs);
  }
  efficiencyFile->Close();
  delete efficiencyFile;
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidHarvester::dqmEndJob(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter) {
  // book the parameters containers
  PPSTimingCalibration::ParametersMap calib_params;
  PPSTimingCalibration::TimingMap calib_time;

  iGetter.cd();
  iGetter.setCurrentFolder(dqmDir_);

  // compute the fit parameters for all monitored channels
  TimingCalibrationHistograms hists;
  std::string ch_name;
  for (const auto& detid : detids_) {
    detid.channelName(ch_name);
    const auto chid = detid.rawId();
    const PPSTimingCalibration::Key key{
        (int)detid.arm(), (int)detid.station(), (int)detid.plane(), (int)detid.channel()};

    PlaneKey plane_key = {(int)detid.arm(), (int)detid.station(), (int)detid.plane()};
    calib_params[key] = {0, 0, 0, 0};
    calib_time[key] = std::make_pair(100'000 * doublePeakLsAndOffset[plane_key].first + doublePeakLsAndOffset[plane_key].second, resolution_);

    hists.leadingTime[chid] = iGetter.get(dqmDir_ + "/t_" + ch_name);
    if (hists.leadingTime[chid] == nullptr) {
      edm::LogInfo("PPSTimingCalibrationPCLValidHarvester:dqmEndJob")
          << "Failed to retrieve leading time monitor for channel (" << detid << ").";
      continue;
    }
    hists.toT[chid] = iGetter.get(dqmDir_ + "/tot_" + ch_name);
    if (hists.toT[chid] == nullptr) {
      edm::LogInfo("PPSTimingCalibrationPCLValidHarvester:dqmEndJob")
          << "Failed to retrieve time over threshold monitor for channel (" << detid << ").";
      continue;
    }
    hists.leadingTimeVsToT[chid] = iGetter.get(dqmDir_ + "/tvstot_" + ch_name);
    if (hists.leadingTimeVsToT[chid] == nullptr) {
      edm::LogInfo("PPSTimingCalibrationPCLValidHarvester:dqmEndJob")
          << "Failed to retrieve leading time vs. time over threshold monitor for channel (" << detid << ").";
      continue;
    }
    if (min_entries_ > 0 && hists.leadingTimeVsToT[chid]->getEntries() < min_entries_) {
      edm::LogWarning("PPSTimingCalibrationPCLValidHarvester:dqmEndJob")
          << "Not enough entries for channel (" << detid << "): " << hists.leadingTimeVsToT[chid]->getEntries() << " < "
          << min_entries_ << ". Skipping calibration.";
      continue;
    }
    // hists.leadingTimeVsLs[chid] = iGetter.get(dqmDir_ + "/tvsls_" + ch_name);
    // if (hists.leadingTimeVsToT[chid] == nullptr) {
    //   edm::LogInfo("PPSTimingCalibrationPCLValidHarvester:dqmEndJob")
    //       << "Failed to retrieve leading time vs. LS monitor for channel (" << detid << ").";
    //   continue;
    // }

    // int double_peak_ls = 1;
    // int n_bins_x = hists.leadingTimeVsLs[chid]->getNbinsX();
    // int n_bins_y = hists.leadingTimeVsLs[chid]->getNbinsY();
    // double prev_t_with_max_count = -1.;
    // double time_shift = 0.;
    // for (int bin_x = 1; bin_x <= n_bins_x; ++bin_x) {
    //   double t_with_max_count = 1;
    //   double max_t_count = 0;
    //   for (int bin_y = 1; bin_y <= n_bins_y; ++bin_y) {
    //     double t_count = hists.leadingTimeVsLs[chid]->getBinContent(bin_x, bin_y);
    //     if (t_count > max_t_count) {
    //       max_t_count = t_count;
    //       t_with_max_count = hists.leadingTimeVsLs[chid]->getTH2F()->GetYaxis()->GetBinCenter(bin_y);
    //     }
    //   }
    //   std::cout << "Max count for LS = " << bin_x << " and t = " << t_with_max_count << ": " << max_t_count << ". Prev max t: " << prev_t_with_max_count << '\n';
    //   if (max_t_count != 0.) {
    //     double time_diff = t_with_max_count - prev_t_with_max_count;
    //     if (prev_t_with_max_count != -1. && abs(time_diff) > double_peak_t_max_diff_) {
    //       double_peak_ls = bin_x;
    //       time_shift = time_diff;
    //       break;
    //     }
    //     prev_t_with_max_count = t_with_max_count;
    //   }
    // }
    // std::cout << "Double peak LS: " << double_peak_ls << " for channel " << detid << '\n';

    // auto validLeadingTime = iBooker.book1D("validt_" + ch_name, ch_name + ";t (ns);Entries", 1200, -60., 60.);
    // for (int bin_x = 1; bin_x <= n_bins_x; ++bin_x) {
    //   for (int bin_y = 1; bin_y <= n_bins_y; ++bin_y) {
    //     double t = hists.leadingTimeVsLs[chid]->getTH2F()->GetYaxis()->GetBinCenter(bin_y);
    //     double t_count = hists.leadingTimeVsLs[chid]->getBinContent(bin_x, bin_y);
    //     if (t_count > 0) {
    //       double t_value = t;
    //       if (bin_x >= double_peak_ls) {
    //         t_value -= time_shift;
    //       }
    //       for (int i = 0; i < t_count; ++i) {
    //         validLeadingTime->Fill(t_value);
    //       }
    //     }
    //   }
    // }

    //find max
    int max_bin_pos = 1;
    for (int i = 0; i < hists.toT[chid]->getNbinsX(); i++) {
      double bin_value = hists.toT[chid]->getBinContent(i);
      int bin_x_pos = hists.toT[chid]->getTH1()->GetXaxis()->GetBinCenter(i);
      if (bin_x_pos > upper_limit_max_search_)
        break;
      if (bin_value > hists.toT[chid]->getBinContent(max_bin_pos))
        max_bin_pos = i;
    }

    std::string ch_name;
    detid.channelName(ch_name);
    auto profile = iBooker.bookProfile(ch_name + "_prof_x", ch_name + "_prof_x", 240, 0., 60., 450, -20., 25.);

    std::unique_ptr<TProfile> prof(hists.leadingTimeVsToT[chid]->getTH2F()->ProfileX("_prof_x", 1, -1));
    *(profile->getTProfile()) = *((TProfile*)prof->Clone());
    profile->getTProfile()->SetTitle(ch_name.c_str());
    profile->getTProfile()->SetName(ch_name.c_str());

    double best_chi_sq_div_ndf = std::numeric_limits<double>::max();
    double best_upper_tot_range = 0.0;
    double best_lower_tot_range = 0.0;
    for (const double upper_threshold_fraction_of_max : thresholds_) {
      for (const double lower_threshold_fraction_of_max : thresholds_) {
        //find ranges if required
        double upper_tot_range = 15;
        if (upper_threshold_fraction_of_max != -1) {
          int upper_limit_pos = max_bin_pos;
          const double upper_threshold = upper_threshold_fraction_of_max * hists.toT[chid]->getBinContent(max_bin_pos);
          while (hists.toT[chid]->getTH1()->GetXaxis()->GetBinCenter(upper_limit_pos) < upper_limit_range_search_) {
            upper_limit_pos++;
            if (hists.toT[chid]->getBinContent(upper_limit_pos) < upper_threshold)
              break;
          }
          upper_tot_range = hists.toT[chid]->getTH1()->GetXaxis()->GetBinCenter(upper_limit_pos);
        }
        double lower_tot_range = 8;
        if (lower_threshold_fraction_of_max != -1) {
          int lower_limit_pos = max_bin_pos;
          const double lower_threshold = lower_threshold_fraction_of_max * hists.toT[chid]->getBinContent(max_bin_pos);
          while (hists.toT[chid]->getTH1()->GetXaxis()->GetBinCenter(lower_limit_pos) > lower_limit_range_search_) {
            lower_limit_pos--;
            if (hists.toT[chid]->getBinContent(lower_limit_pos) < lower_threshold)
              break;
          }
          lower_tot_range = hists.toT[chid]->getTH1()->GetXaxis()->GetBinCenter(lower_limit_pos);
        }

        interp_.SetParameters(hists.leadingTime[chid]->getRMS(),
                              hists.toT[chid]->getMean(),
                              0.8,
                              hists.leadingTime[chid]->getMean() - hists.leadingTime[chid]->getRMS());
        const auto& res = profile->getTProfile()->Fit(&interp_, "BNS", "", lower_tot_range, upper_tot_range);
        if (res->IsValid()) {
          double chi_sq_div_ndf = res->Chi2() / res->Ndf();
          if (chi_sq_div_ndf < best_chi_sq_div_ndf) {
            best_chi_sq_div_ndf = chi_sq_div_ndf;
            best_upper_tot_range = upper_tot_range;
            best_lower_tot_range = lower_tot_range;
          }
        }
      }
    }

    if (best_upper_tot_range != 0.0) {
      profile->getTProfile()->Fit(&interp_, "B", "", best_lower_tot_range, best_upper_tot_range);
      calib_params[key] = {
          interp_.GetParameter(0), interp_.GetParameter(1), interp_.GetParameter(2), interp_.GetParameter(3)};
      calib_time[key] =
          std::make_pair(offset_, resolution_);  // hardcoded offset/resolution placeholder for the time being
    } else {
        edm::LogWarning("PPSTimingCalibrationPCLValidHarvester:dqmEndJob")
            << "Fit did not converge for channel (" << detid << ").";
    }
  }

  // fill the DB object record
  PPSTimingCalibration calib(formula_, calib_params, calib_time);

  // write the object
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  poolDbService->writeOneIOV(calib, poolDbService->currentTime(), "PPSTimingCalibrationRcd_HPTDC");
}

//------------------------------------------------------------------------------

void PPSTimingCalibrationPCLValidHarvester::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("dqmDir", "AlCaReco/PPSTimingCalibrationPCL")
      ->setComment("input path for the various DQM plots");
  desc.add<std::string>("formula", "[0]/(exp((x-[1])/[2])+1)+[3]")
      ->setComment("interpolation formula for the time walk component");
  desc.add<unsigned int>("minEntries", 100)->setComment("minimal number of hits to extract calibration");
  desc.add<std::string>("tVsLsFileName", "DQM_V0001_R000385281__CalibPPS__TimingCalibration__CMSSW_13_3_0.root")
      ->setComment("t vs LS ROOT filename");
  descriptions.addWithDefaultLabel(desc);
}

DEFINE_FWK_MODULE(PPSTimingCalibrationPCLValidHarvester);
