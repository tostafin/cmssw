name=$1
cmsRun universalConfig.py $name
cmsShow -c ./Fireworks/Core/macros/simGeo.fwc --sim-geom-file $name.root
