void aliases(TTree* Events)
{
  //TChain * Events = new TChain("Events","Events")
  //Events->Add("/d100/data/MinimumBias-ReReco/Jan29ReReco-v2/bambu/BSC*.root")

  // === Event info ===
  Events->SetAlias("run","EventHeader.fRunNum");
  Events->SetAlias("goodRuns","((run>=123596 && run<=124030) || run==124120)");
  Events->SetAlias("vtx","PrimaryVertexes.fArray");
  Events->SetAlias("goodVertex","(vtx.fNdof>5 && abs(vtx.Z())<15)");

  // === trigger ===
  //  - L1T -
  Events->SetAlias("L1T","L1TechBitsBeforeMask.fMask");
  Events->SetAlias("L1T36","L1T.TestBit(36)");
  Events->SetAlias("L1T37","L1T.TestBit(37)");
  Events->SetAlias("L1T38","L1T.TestBit(38)");
  Events->SetAlias("L1T39","L1T.TestBit(39)");
  Events->SetAlias("haloVeto","!(L1T36||L1T37||L1T38||L1T39)");
  Events->SetAlias("L1T40","L1T.TestBit(40)");
  Events->SetAlias("L1T41","L1T.TestBit(41)");
  //  - L1A -
  Events->SetAlias("L1A","L1AlgoBitsBeforeMask.fMask");
  Events->SetAlias("L1A0","L1A.TestBit(0)");
  Events->SetAlias("L1A82","L1A.TestBit(82)");
  //  - HLT -

  // === jets ===
  //  - uncorr -
  //    Pt(), P(), Et(), Mass(), Eta(), Phi(), E(), NConstituents()==NTowers()
  //    fEnergyFractionEm
  Events->SetAlias("jets","AKt5Jets.fArray");
  //Events->SetAlias("jets","ItrCone5Jets.fArray");
  Events->SetAlias("jetsEmf","jets.fEnergyFractionEm");
  // for *Un*corrected jets, the cut is ~ 0.5 of jetmet cut on corrPt
  Events->SetAlias("goodJets","(jets.Pt()>7 && jets.Pt()<60 && jetsEmf>0.01 && abs(jets.Eta())<2)");

  // === tracks
  //   Pt(), Eta(), Phi(), Charge(), 
  Events->SetAlias("ntrk","Tracks.Entries()");
  Events->SetAlias("trk","Tracks.fArray");

  // sigXY = quadratic sum of d0Err and beamspot width
  Events->SetAlias("sigXY","sqrt(trk.D0Err()*trk.D0Err() + 0.04*0.04)");

  // high purity tracks
  Events->SetAlias("highPurity","trk.fQuality.fQualityMask.fBitMask.TestBit(2)");

  // approximation of ptErr ignoring covariance term
  Events->SetAlias("ptErr","sqrt(pow(trk.P()*trk.Pt()*trk.fQOverPErr/trk.Charge(),2) + pow(trk.Pz()*trk.fLambdaErr,2))");


  // - ok for now keep it simple.
  //   * From the JPT talk we see that the sigma pt, dxy, dz cuts have high efficiency
  Events->SetAlias("goodTracks","tracks.Pt()>0.3 && tracks.Pt()<20 && highPurity");
}
