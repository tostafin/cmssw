#include "CondFormats/RunInfo/interface/LHCInfoCombined.h"

LHCInfoCombined::LHCInfoCombined(const LHCInfo& lhcInfo) {
  setFromLHCInfo(lhcInfo);
}
  
LHCInfoCombined::LHCInfoCombined(const LHCInfoPerLS& infoPerLS, const LHCInfoPerFill& infoPerFill) {
  setFromPerLS(infoPerLS);
  setFromPerFill(infoPerFill);
}

LHCInfoCombined::LHCInfoCombined(const edm::EventSetup &iSetup, 
      const edm::ESGetToken<LHCInfoPerLS, LHCInfoPerLSRcd>& tokenInfoPerLS,
      const edm::ESGetToken<LHCInfoPerFill, LHCInfoPerFillRcd>& tokenInfoPerFill,
      const edm::ESGetToken<LHCInfo, LHCInfoRcd>& tokenInfo) {
  if(true /* era run3 */) {
    edm::ESHandle<LHCInfoPerLS> hLHCInfoPerLS = iSetup.getHandle(tokenInfoPerLS);
    edm::ESHandle<LHCInfoPerFill> hLHCInfoFill = iSetup.getHandle(tokenInfoPerFill);
    setFromPerLS(*hLHCInfoPerLS);
    setFromPerFill(*hLHCInfoFill);
  } else {
    edm::ESHandle<LHCInfo> hLHCInfo = iSetup.getHandle(tokenInfo);
    setFromLHCInfo(*hLHCInfo);
  }
}

void LHCInfoCombined::setFromLHCInfo(const LHCInfo& lhcInfo) {
  setCrossingAngleX(lhcInfo.crossingAngle());
  setCrossingAngleY(0);
  setBetaStarX(lhcInfo.betaStar());
  setBetaStarY(lhcInfo.betaStar());
  setEnergy(lhcInfo.energy());
}
void LHCInfoCombined::setFromPerLS(const LHCInfoPerLS& infoPerLS) {
  setCrossingAngleX(infoPerLS.crossingAngleX());
  setCrossingAngleY(infoPerLS.crossingAngleY());
  setBetaStarX(infoPerLS.betaStarX());
  setBetaStarY(infoPerLS.betaStarY());
}
void LHCInfoCombined::setFromPerFill(const LHCInfoPerFill& infoPerFill) {
  setEnergy(infoPerFill.energy());
}

//getters
float const LHCInfoCombined::crossingAngleX() const { return crossingAngleX_; }

float const LHCInfoCombined::crossingAngleY() const { return crossingAngleY_; }

float const LHCInfoCombined::betaStarX() const { return betaStarX_; }

float const LHCInfoCombined::betaStarY() const { return betaStarY_; }

float const LHCInfoCombined::energy() const { return energy_; }

//setters
void LHCInfoCombined::setCrossingAngleX(float const& angleX) { crossingAngleX_ = angleX; }

void LHCInfoCombined::setCrossingAngleY(float const& angleY) { crossingAngleY_ = angleY; }

void LHCInfoCombined::setBetaStarX(float const& betaStarX) { betaStarX_ = betaStarX; }

void LHCInfoCombined::setBetaStarY(float const& betaStarY) { betaStarY_ = betaStarY; }

void LHCInfoCombined::setEnergy(float const& energy) { energy_ = energy; }