class GammaJet {
 public:
  GammaJet() :
    photonEt(-99),photonRawEt(-99),
    photonEta(9999),photonPhi(9999),
    hovere(10000),
    sigmaIetaIeta(-99),
    sumIsol(99999),
    genIso(99999),
    genMomId(-999999),
    lJetPt(-999990),
    lJetEta(-999990),
    lJetPhi(-999990)


      {}
    float photonEt,photonRawEt;
    float photonEta;
    float photonPhi;
    float hovere,sigmaIetaIeta, sumIsol;
    float genIso;
    int genMomId;
    float lJetPt;
    float lJetEta;
    float lJetPhi;

    void clear() {
      photonRawEt=-99; photonEt = -99999; photonEta=100; photonPhi=1000;
      sigmaIetaIeta=-99;
      sumIsol = -99;
      genIso = 9990;
      genMomId = -999999;
      hovere  =  1000;
      lJetPt = -99999;
      lJetEta = -99999;
      lJetPhi = -99999;

    }
};