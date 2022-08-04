// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"

class TotemT2DigiAnalyzer : public edm::one::EDAnalyzer<> {
public:
  explicit TotemT2DigiAnalyzer(const edm::ParameterSet&);
//   ~TotemT2DigiAnalyzer();

//   static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override; 
  
  edm::EDGetTokenT<edm::DetSetVector<TotemT2Digi>> digiToken_;
  std::vector<std::vector<unsigned int>> testCasesVec;
  unsigned int numberOfAllEvents;

  std::unordered_map<unsigned int, unsigned int> ids_counters_;
  std::unordered_map<unsigned int, unsigned int> event_counters_;

};

TotemT2DigiAnalyzer::TotemT2DigiAnalyzer(const edm::ParameterSet& iConfig) : 
        digiToken_(consumes<edm::DetSetVector<TotemT2Digi>>(iConfig.getParameter<edm::InputTag>("digisTag"))),
        numberOfAllEvents(iConfig.getParameter<unsigned int>("numberOfAllEvents")) {

    for (const auto& ids : iConfig.getParameter<std::vector<edm::ParameterSet>>("testCasesSet")){
        testCasesVec.push_back(ids.getParameter<std::vector<unsigned int>>("detId"));
    }

}

void TotemT2DigiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::LogWarning("analyze job") << "whatsup";
    for (const auto& ds_digis : iEvent.get(digiToken_)) {
        const TotemT2DetId detid(ds_digis.detId());
        for (const auto& digi : ds_digis) {
            event_counters_[detid]++;
            (void)digi;
        }
  }

}

// ------------ method called once each job just before starting event loop  ------------
void TotemT2DigiAnalyzer::beginJob() {
    edm::LogWarning("Begin job") << "hello";
    for(auto vec : testCasesVec){
        for (const auto& id : vec) {
            // count how many times each id appeared in test cases
            if(ids_counters_.find(id) != ids_counters_.end()){
                ids_counters_[id]++;
            }else{
                ids_counters_[id] = 0;
            }
            // initialize event counter for each id
            event_counters_[id] = 0;
        }
    }
}

// ------------ method called once each job just after ending the event loop  ------------
void TotemT2DigiAnalyzer::endJob() {
    for(auto vec : testCasesVec){
        for (const auto& id : vec) {
            edm::LogWarning("End job summary: ") << "there should be " << numberOfAllEvents << " events, there is " << event_counters_[id] * ids_counters_[id] << " events";
            // ids_counters[id];
            // event_counters_[id];
        }
    }
}

// // ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
// void TotemT2DigiAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
//   //The following says we do not know what parameters are allowed so do no validation
//   // Please change this to state exactly what you do use, even if it is no parameters
// //   edm::ParameterSetDescription desc;
// //   desc.setUnknown();
// //   descriptions.addDefault(desc);

//   //Specify that only 'tracks' is allowed
//   //To use, remove the default given above and uncomment below
//   //ParameterSetDescription desc;
//   //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
//   //descriptions.addWithDefaultLabel(desc);
// }

DEFINE_FWK_MODULE(TotemT2DigiAnalyzer);
