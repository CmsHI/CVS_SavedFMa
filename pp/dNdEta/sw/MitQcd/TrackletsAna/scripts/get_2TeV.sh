# pythia
hadd pixelTree_Pythia_MinBias_D6T_2360GeV_d20091229_Vertex1224.root /d101/yetkin/mc/CMSSW_3_3_5/Vertex1224/Pythia_MinBias_D6T_2360GeV_d20091229/HLTv17V1/pixeltree/Pythia_MinBias_D6T_2360GeV_d20091229_00*.root >&  log/pixelTree_Pythia_MinBias_D6T_2360GeV_d20091229_Vertex122.txt

# phojet
cp /d101/yetkin/mc/CMSSW_3_3_5/Phojet_MinBias_2360GeV_d20100108/HLTv13V1/pixeltree/Phojet_MinBias_2360GeV_d20100108.root pixelTree_pixelTree_Pythia_MinBias_D6T_2360GeV_d20091229_Vertex1224.root

# data
cp /home/yjlee/ana/trackletStudy/trackletAna/newUltimateSample/PixelTree-124120-hfcuts.root .
