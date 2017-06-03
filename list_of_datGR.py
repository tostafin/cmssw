## raw data source
import glob
import os


pathMinidaq='/store/t0streamer/Minidaq/A'
pathGR='/store/t0streamer/Data/Express'
pathSelected=pathGR


print "*("
pathEosMounted='/afs/cern.ch/user/n/nminafra/Eos/cms'
#filepath='/afs/cern.ch/user/n/nminafra/Eos/cms/store/t0streamer/Minidaq/A/000/FIRSTTHREE/LASTTHREE/'
#filepath='/afs/cern.ch/user/n/nminafra/Eos/cms/store/t0streamer/Data/Physics/000/FIRSTTHREE/LASTTHREE/'
filepathEos=pathEosMounted + pathSelected + '/000/FIRSTTHREE/LASTTHREE/'
filepath= pathSelected + '/000/FIRSTTHREE/LASTTHREE/'
onlynames_dat= [os.path.basename(x) for x in glob.glob(filepathEos+'*.dat')]
for i,filename_dat in enumerate(onlynames_dat):
    #onlynames_dat[i] ='\'/store/t0streamer/Minidaq/A/000/FIRSTTHREE/LASTTHREE/' + filename_dat + '\','
    onlynames_dat[i] = '\'' + filepath + filename_dat + '\','
    if i<254000: print onlynames_dat[i]
print ")"
