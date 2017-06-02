## raw data source
import glob
import os
print "*("
filepath='/afs/cern.ch/user/n/nminafra/Eos/cms/store/t0streamer/Minidaq/A/000/295/627/'
onlynames_dat= [os.path.basename(x) for x in glob.glob(filepath+'*.dat')]
for i,filename_dat in enumerate(onlynames_dat):
    onlynames_dat[i] ='\'/store/t0streamer/Minidaq/A/000/295/627/' + filename_dat + '\','
    if i<254000: print onlynames_dat[i]
print ")"

