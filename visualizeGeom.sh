name=${1-geomCTPPS}
cmsRun universalConfig.py $name
cmsShow -s -c ./Fireworks/Core/macros/simGeo.fwc --sim-geom-file $name.root
