#
# Usage:
#   comment out the find line
#   then execute script followed by pipe to bash:
#   ./Ana_NearAwayEtCut_DPhiCut_combine.sh | sh

#--- Scout landscape ---
find /net/hisrv0001/home/frankma/work/hiroot/test/dijet/jetana/plots/prodPtHat50/iCone5_match35_jetEtCut30_ntJetEt -type d -maxdepth 1 | grep test_300k_pythia_90_110_50

#--- for Pyquen Pt scan ---
./combinePlots.sh 300k_pythia_90_110_50_.*_3\.0 chCheckNearLJetPythia
./combinePlots.sh 300k_pythia_90_110_50_.*_3\.0 chXiNearJetPythia
./combinePlots.sh 300k_pythia_90_110_50_.*_3\.0 chXiAwayJetPythia
./combinePlots.sh 300k_pythia_90_110_50_.*_3\.0 chNearFFRatio
#--- for DPhi scan ---
./combinePlots.sh 300k_pythia_90_110_50_pyquen_90_110_50_.\.. chCheckNearLJetPythia
./combinePlots.sh 300k_pythia_90_110_50_pyquen_90_110_50_.\.. chXiNearJetPythia
./combinePlots.sh 300k_pythia_90_110_50_pyquen_90_110_50_.\.. chXiAwayJetPythia
./combinePlots.sh 300k_pythia_90_110_50_pyquen_90_110_50_.\.. chNearFFRatio

