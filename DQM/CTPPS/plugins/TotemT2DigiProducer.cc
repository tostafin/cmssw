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
    
    // CONSTANTS
    // only channel 4 contains meaningful data
    const short channelNumber_ = 4;
    // dummy chars just to satisfy TotemT2Digi constructor
    const unsigned char dummyGeo_ = 'g';
    const unsigned char dummyId_ = 'i';
    const unsigned char dummyMarker_ = 'm';
    // conversion constant between time slice and absolute time (in ns) - but inverted
    const double timeSliceNsInverted_ = 1024.0 / 25.0;
    // conversion constant between nanoseconds and seconds
    const double nsToS_ = std::pow(10, 9);

};

TotemT2DigiProducer::TotemT2DigiProducer(const edm::ParameterSet& iConfig) : t2FilesVec_(iConfig.getParameter<std::vector<std::string>>("t2FilesVec")){
    for (const auto& id_map : iConfig.getParameter<std::vector<edm::ParameterSet>>("idsMapping")){
        unsigned int treeChId = id_map.getParameter<unsigned int>("treeChId");
        detid_vs_chid_[treeChId] = id_map.getParameter<std::vector<unsigned int>>("detId");
    }

    inputTree_ = std::make_unique<TChain>("tree");

    for (const auto& fname : t2FilesVec_){
        inputTree_->Add(fname.c_str());
    }

    produces<edm::DetSetVector<TotemT2Digi>>("TotemT2");
}

void TotemT2DigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
    int eventNum = iEvent.id().event();
    auto digi = std::make_unique<edm::DetSetVector<TotemT2Digi>>();

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

    if(channelNo == channelNumber_){
        unsigned short le = (unsigned short) floor(leadingEdge * nsToS_ * timeSliceNsInverted_);
        unsigned short te = (unsigned short) floor(trailingEdge * nsToS_ * timeSliceNsInverted_);
        TotemT2Digi digiTmp(dummyGeo_, dummyId_, dummyMarker_, le, te); 

        auto vec = detid_vs_chid_.at(channelNo);
        for (const auto& id : vec) {
            TotemT2DetId detId(id);
            edm::DetSet<TotemT2Digi>& digis_for_detid = digi->find_or_insert(detId);
            digis_for_detid.push_back(digiTmp);
        }
    }

    iEvent.put(std::move(digi), "TotemT2");
}

void TotemT2DigiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<std::vector<std::string>>("t2FilesVec")->setComment("path to fake T2 root data");

    edm::ParameterSetDescription idmap_valid;
    idmap_valid.add<unsigned int>("treeChId", 0)->setComment("Channel id retrieved from tree");
    idmap_valid.add<std::vector<unsigned int>>("detId")->setComment("mapped TotemT2DetId's for this channel");

    desc.addVPSet("idsMapping", idmap_valid);

    descriptions.add("TotemT2DigiProducer", desc);
}

DEFINE_FWK_MODULE(TotemT2DigiProducer);