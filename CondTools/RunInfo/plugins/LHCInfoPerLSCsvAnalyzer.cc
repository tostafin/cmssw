#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CondFormats/RunInfo/interface/LHCInfoPerLS.h"
#include "CondFormats/DataRecord/interface/LHCInfoPerLSRcd.h"

#include <memory>
#include <iostream>
#include <cassert>

class LHCInfoPerLSCsvAnalyzer : public edm::one::EDAnalyzer<> {
public:
  explicit LHCInfoPerLSCsvAnalyzer(const edm::ParameterSet&)
      : tokenInfoPerLS_(esConsumes<LHCInfoPerLS, LHCInfoPerLSRcd>()) {}

private:
  void beginJob() override {}
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override {}

  edm::ESWatcher<LHCInfoPerLSRcd> infoPerLSWatcher_;

  edm::ESGetToken<LHCInfoPerLS, LHCInfoPerLSRcd> tokenInfoPerLS_;
};

void LHCInfoPerLSCsvAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  if (infoPerLSWatcher_.check(iSetup)) {
    const LHCInfoPerLS& infoPerLS = iSetup.getData(tokenInfoPerLS_);
    std::cout << "LHCInfoPerLS;"
      << iEvent.time().unixTime() << ";"
      << infoPerLS.runNumber() << ";" 
      << infoPerLS.lumiSection() << ";" 
      << infoPerLS.crossingAngleX() << ";"
      << infoPerLS.crossingAngleY() << ";"
      << infoPerLS.betaStarX() << ";"
      << infoPerLS.betaStarY() << ";"
      << std::endl;
  }
}

DEFINE_FWK_MODULE(LHCInfoPerLSCsvAnalyzer);
