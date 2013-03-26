# genjet analysis
# echo gluon genjet ================================================
# cat drawInclJetFrag_flavor.C | sed 's/void drawInclJetFrag_flavor()/void drawInclJetFrag_tmp()/' | sed 's/SetAlias("finalJetPt","jetPt")/SetAlias("finalJetPt","jetPtGM")/' | sed 's/_gluon_/_gluon_genjet_/g' > drawInclJetFrag_tmp.C
# root -b -q drawInclJetFrag_tmp.C+

# echo quark genjet ================================================
# cat drawInclJetFrag_flavor.C | sed 's/void drawInclJetFrag_flavor()/void drawInclJetFrag_tmp()/' | sed 's/SetAlias("finalJetPt","jetPt")/SetAlias("finalJetPt","jetPtGM")/' | sed 's/abs(partonFlav)==21/abs(partonFlav)<=6/' | sed 's/_gluon_/_quark_genjet_/g' > drawInclJetFrag_tmp.C
# root -b -q drawInclJetFrag_tmp.C+

# echo stdana genjet ================================================
# cat drawInclJetFrag.C | sed 's/void drawInclJetFrag()/void drawInclJetFrag_tmp()/'               | sed 's/SetAlias("finalJetPt","jetPt")/SetAlias("finalJetPt","jetPtGM")/' | sed 's/_mc80to170_/_mc80to170_genjet_/g' > drawInclJetFrag_tmp.C
# root -b -q drawInclJetFrag_tmp.C+

# echo stdana ================================================
# root -b -q drawInclJetFrag.C+

# flavor dependence
# root -b -q drawInclJetFrag_flavor.C+
# cat drawInclJetFrag_flavor.C | sed 's/void drawInclJetFrag_flavor()/void drawInclJetFrag_tmp()/' | sed 's/abs(partonFlav)==21/abs(partonFlav)<=6/' | sed 's/_gluon_/_quark_/g' > drawInclJetFrag_tmp.C
# root -b -q drawInclJetFrag_tmp.C+

# pt dependence
cat drawInclJetFrag.C | sed 's/void drawInclJetFrag()/void drawInclJetFrag_tmp()/' | sed 's/float finalJetPtMin = 100;/float finalJetPtMin = 100;/' | sed 's/float finalJetPtMax = 300;/float finalJetPtMax = 120;/' > drawInclJetFrag_tmp.C
root -b -q drawInclJetFrag_tmp.C+
cat drawInclJetFrag.C | sed 's/void drawInclJetFrag()/void drawInclJetFrag_tmp()/' | sed 's/float finalJetPtMin = 100;/float finalJetPtMin = 120;/' | sed 's/float finalJetPtMax = 300;/float finalJetPtMax = 150;/' > drawInclJetFrag_tmp.C
root -b -q drawInclJetFrag_tmp.C+
cat drawInclJetFrag.C | sed 's/void drawInclJetFrag()/void drawInclJetFrag_tmp()/' | sed 's/float finalJetPtMin = 100;/float finalJetPtMin = 150;/' | sed 's/float finalJetPtMax = 300;/float finalJetPtMax = 200;/' > drawInclJetFrag_tmp.C
root -b -q drawInclJetFrag_tmp.C+
cat drawInclJetFrag.C | sed 's/void drawInclJetFrag()/void drawInclJetFrag_tmp()/' | sed 's/float finalJetPtMin = 100;/float finalJetPtMin = 200;/' | sed 's/float finalJetPtMax = 300;/float finalJetPtMax = 300;/' > drawInclJetFrag_tmp.C
root -b -q drawInclJetFrag_tmp.C+