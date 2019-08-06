import ROOT
import sys
from DataFormats.FWLite import Events, Handle 

#In this case, I'll use a single input file
events = Events ('AOD.root')

#We then create a handle. Unlike cmsRun and FWLite in C++, this is a cpu-intensive operation so it is recommend to do this outside of the event loop.
#handle  = Handle ('std::vector<pat::Muon>')
handle = Handle("edm::DetSetVector<CTPPSDiamondRecHit>")

#detId=ROOT.CTPPSDiamondDigi();
#handle = Handle("edm::det_id_type")

#Now we make a label like an edm::InputTag
label = ("ctppsDiamondRecHits",)
ROOT.gROOT.SetBatch()        # don't pop up canvases
# Create histograms, etc.
ROOT.gROOT.SetStyle('Plain') # white background
#zmassHist = ROOT.TH1F ("zmass", "Z Candidate Mass", 50, 20, 220)

# loop over events
import pprint
pp = pprint.PrettyPrinter(indent=4)

for event in events:
	event.getByLabel (label, handle)
	#pp.pprint(event)
	# get the product
	digis = handle.product()
	for ds in digis:
		if ds.size() == 0:
			continue
		detid = ROOT.CTPPSDiamondDetId(ds.detId())
#		pp.pprint(dir(detid))
		print detid.arm(), detid.station(), detid.rp(), detid.plane(), detid.channel()
		for digi in ds:
			print 'X', digi.getX()
			print 'Y', digi.getY()
			print 'Z', digi.getZ()
	# use muons to make Z peak
#	numMuons = len (muons)
#		if muons < 2: continue
#		for outer in xrange (numMuons - 1):
#			outerMuon = muons[outer]
#			for inner in xrange (outer + 1, numMuons):
#				innerMuon = muons[inner]
#				if outerMuon.charge() * innerMuon.charge() >= 0:
#		continue
#		  inner4v = ROOT.TLorentzVector (innerMuon.px(), innerMuon.py(),
#				  innerMuon.pz(), innerMuon.energy())
#				outer4v = ROOT.TLorentzVector (outerMuon.px(), outerMuon.py(),
#						outerMuon.pz(), outerMuon.energy())
#				zmassHist.Fill( (inner4v + outer4v).M() )
# make a canvas, draw, and save it
#c1 = ROOT.TCanvas()
#zmassHist.Draw()
#c1.Print ("zmass_py.png")
