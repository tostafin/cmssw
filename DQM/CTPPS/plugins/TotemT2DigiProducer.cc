// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/TotemT2DetId.h"
#include "DataFormats/TotemReco/interface/TotemT2Digi.h"

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

class TotemT2DigiProducer : public edm::stream::EDProducer<>{
public:
    explicit TotemT2DigiProducer(const edm::ParameterSet&);

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    void produce(edm::Event&, const edm::EventSetup&) override;

    std::vector<std::string> t2FilesVec_;
    std::unordered_map<unsigned int, std::vector<unsigned int>> detid_vs_chid_;
    std::unique_ptr<TChain> inputTree_;

};

TotemT2DigiProducer::TotemT2DigiProducer(const edm::ParameterSet& iConfig) : t2FilesVec_(iConfig.getParameter<std::vector<std::string>>("t2FilesVec")){
    for (const auto& id_map : iConfig.getParameter<std::vector<edm::ParameterSet>>("idsMapping")){
        unsigned int treeChId = id_map.getParameter<unsigned int>("treeChId");
        detid_vs_chid_[treeChId] = id_map.getParameter<std::vector<unsigned int>>("detId");
        // edm::LogWarning("Totem T2 digi producer ") << treeChId;
    }
    inputTree_ = std::make_unique<TChain>("tree");

    for (const auto& fname : t2FilesVec_){
        // edm::LogWarning("Totem T2 digi producer filename ") << fname;
        inputTree_->Add(fname.c_str());
    }

    produces<edm::DetSetVector<TotemT2Digi>>("TotemT2");
}

void TotemT2DigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
    int eventNum = iEvent.id().event();
    auto digi = std::make_unique<edm::DetSetVector<TotemT2Digi>>();

    // 'eventNo', 'numberOfChannels', 'channelNo', 'MaxAmplitude', 'LeadingEdge', 'TrailingEdge', 'TOT'
    int eventNo;
    int numberOfChannels;
    int channelNo;
    double maxAmplitude;
    double leadingEdge;
    double trailingEdge;
    double tot;

    inputTree_->SetBranchAddress("eventNo", &eventNo);
    inputTree_->SetBranchAddress("numberOfChannels", &numberOfChannels);
    inputTree_->SetBranchAddress("channelNo", &channelNo);
    inputTree_->SetBranchAddress("MaxAmplitude", &maxAmplitude);
    inputTree_->SetBranchAddress("LeadingEdge", &leadingEdge);
    inputTree_->SetBranchAddress("TrailingEdge", &trailingEdge);
    inputTree_->SetBranchAddress("TOT", &tot);
    inputTree_->GetEntry(eventNum);

    edm::LogWarning("Totem T2 digi producer produce") << eventNo << ' ' << numberOfChannels << ' ' << channelNo << ' ' << maxAmplitude << ' ' << leadingEdge << ' ' << trailingEdge << ' ' << tot;
    // Works up to here
    if(channelNo == 4){
        TotemT2Digi digiTmp(); // TODO 

        auto vec = detid_vs_chid_.at(ch_id);
        for (const auto& id : vec) {
            TotemT2DetId detId(id);
            edm::DetSet<TotemT2Digi>& digis_for_detid = digi->find_or_insert(detId);
            digis_for_detid.push_back(digiTmp);
        }
    }
    iEvent.put(std::move(digi), "totemT2Digis");
}

void TotemT2DigiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    // TODO
}

DEFINE_FWK_MODULE(TotemT2DigiProducer);