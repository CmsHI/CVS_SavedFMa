# pythia
hadd pixelTree_Pythia_MinBias_D6T_900GeV_d20091229_Vertex1229.root /d101/yetkin/mc/CMSSW_3_3_5/Vertex1229/Pythia_MinBias_D6T_900GeV_d20091229/HLTv17V1/pixeltree/Pythia_MinBias_D6T_900GeV_d20091229_*.root >& log/merge_pixelTree_Pythia_MinBias_D6T_900GeV_d20091229_Vertex1229.txt

# phojet
cp /d101/yetkin/mc/CMSSW_3_3_5/Vertex1229/Phojet_MinBias_900GeV_d20100108/HLTv13V1/pixeltree/Phojet_MinBias_900GeV_d20100108.root .

# data
cp /home/yjlee/ana/trackletStudy/trackletAna/newUltimateSample/PixelTree-124022-hfcuts.root .
