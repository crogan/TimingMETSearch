/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_X2X2_to_ZllXHggX.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Nov
//
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "RestFrames/RestFrames.hh"

#include "Detector.hh"
#include "Physics.hh"
#include "Resolution.hh"
#include <TSystem.h>
#include <TROOT.h>
#include <TKey.h>

using namespace RestFrames;

void Mass_LLP_Detector_X2X2_to_ZllXZllX(std::string output_name =
			      "output_Mass_LLP_Detector_X2X2_to_ZallXZbllX.root"){

    Long64_t start = gSystem->Now();
    double ctau = 50.0;
    double mX1 = 300.0;
    double mZ = 91.19;
    double wZ = 2.50;
  
    
    vector<double> mX2;
    //mX2.push_back(1000.);
    //mX2.push_back(2000.);
    //mX2.push_back(3000.);
    //mX2.push_back(4000.);
    mX2.push_back(5000.);
    
    
    int NmX2 = mX2.size();

    //Number of events
    int Ngen = 100000;
    
    g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
    
    //
    ppLabGenFrame LAB_Gen("LAB_Gen","LAB");
    DecayGenFrame     X2X2_Gen("X2X2_Gen","#tilde{#chi}^{ 0}_{2} #tilde{#chi}^{ 0}_{2}");
    DecayGenFrame     X2a_Gen("X2a_Gen","#tilde{#chi}^{ 0}_{2 a}");
    DecayGenFrame     X2b_Gen("X2b_Gen","#tilde{#chi}^{ 0}_{2 b}");
    ResonanceGenFrame Za_Gen("Za_Gen","Z_{a}");
    ResonanceGenFrame Zb_Gen("Zb_Gen","Z_{b}");
    VisibleGenFrame   L1a_Gen("L1a_Gen","#it{l}_{1a}");
    VisibleGenFrame   L2a_Gen("L2a_Gen","#it{l}_{2a}");
    VisibleGenFrame   L1b_Gen("L1b_Gen","#it{l}_{1b}");
    VisibleGenFrame   L2b_Gen("L2b_Gen","#it{l}_{2b}");
    InvisibleGenFrame X1a_Gen("X1a_Gen","#tilde{#chi}^{ 0}_{1 a}");
    InvisibleGenFrame X1b_Gen("X1b_Gen","#tilde{#chi}^{ 0}_{1 b}");
    
    //
    LAB_Gen.SetChildFrame(X2X2_Gen);
    X2X2_Gen.AddChildFrame(X2a_Gen);
    X2X2_Gen.AddChildFrame(X2b_Gen);
    X2a_Gen.AddChildFrame(Za_Gen);
    X2a_Gen.AddChildFrame(X1a_Gen);
    X2b_Gen.AddChildFrame(Zb_Gen);
    X2b_Gen.AddChildFrame(X1b_Gen);
    Za_Gen.AddChildFrame(L1a_Gen);
    Za_Gen.AddChildFrame(L2a_Gen);
    Zb_Gen.AddChildFrame(L1b_Gen);
    Zb_Gen.AddChildFrame(L2b_Gen);
    
    if(LAB_Gen.InitializeTree())
        g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
    else
        g_Log << LogError << "...Failed initializing generator tree" << LogEnd;
    
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
    
  
  X2X2_Gen.SetVariableMass();
  X2a_Gen.SetMass(mX2[0]);       X2b_Gen.SetMass(mX2[0]);
  X1a_Gen.SetMass(mX1);       X1b_Gen.SetMass(mX1);
  Za_Gen.SetMass(mZ);
  Za_Gen.SetWidth(wZ);
  Zb_Gen.SetMass(mZ);
  Zb_Gen.SetWidth(wZ);
  /*
  L1a_Gen.SetPtCut(10.);        L1a_Gen.SetEtaCut(2.5);
  L2a_Gen.SetPtCut(10.);        L2a_Gen.SetEtaCut(2.5);
  L1b_Gen.SetPtCut(10.);        L1b_Gen.SetEtaCut(2.5);
  L2b_Gen.SetPtCut(10.);        L2b_Gen.SetEtaCut(2.5);
  */
  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  ////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree", true);
  
    // Declare observables for histogram booking
    HistPlot* histPlot = new HistPlot("Plots",
                                      std::string("#tilde{#chi}_{2}^{ 0} #tilde{#chi}_{2}^{ 0}") +
                                      "#rightarrow Za(#it{l}#it{l}) #tilde{#chi}_{1}^{ 0}"+
                                      "Zb(#it{l}#it{l}) #tilde{#chi}_{1}^{ 0}");
    
    histPlot->SetRebin(1);
    
    RFList<const HistPlotCategory> cat_list;
    for(int m = 0; m < NmX2; m++)
    {
        char smX2[200], scat[50];
        sprintf(scat, "MX2_%.0f", mX2[m]);
        sprintf(smX2, "m_{#tilde{#chi}^{0}_{2}}= %.0f", mX2[m]);
        
        cat_list += histPlot->GetNewCategory(scat, std::string(smX2));
    }
    
    //setting up all the variables that could be potentially plotted
    const HistPlotVar& Pull_Mass_Parent = histPlot->GetNewVar("Pull_Mass_Parent","Pull of M(#tilde{#chi}_{2a}^{0})",-5.0,5.0,"");
    //const HistPlotVar& Mass_Parent_Resolution = histPlot->GetNewVar("Resolution_of_Parent_Mass","M(#tilde{#chi}_{2a}^{0})",-200.0,200.0,"[GeV]");
    const HistPlotVar& MassReco_MassGen = histPlot->GetNewVar("RecoMass_GenMass","#Delta M(#tilde{#chi}_{2a}^{0})",-100.0,100.0,"[GeV]");
    const HistPlotVar& Pull_E_Z = histPlot->GetNewVar("Pull_E_Z","Pull of #delta E_{Za}^{#tilde{#chi}_{2a}^{0}}",-5.0,5.0,"");
    const HistPlotVar& Pull_E_L = histPlot->GetNewVar("Pull_E_L","Pull of #delta E_{L1a}",-5.0,5.0,"");
    const HistPlotVar& Pull_D = histPlot->GetNewVar("Pull_D","Pull of Distance",-5.0,5.0,"");
    const HistPlotVar& Pull_T = histPlot->GetNewVar("Pull_T","Pull of Time",-5.0,5.0,"");
    const HistPlotVar& Pull_Beta_Mag = histPlot->GetNewVar("Pull_Beta_Mag","Pull of |#vec{#Beta}|",-5.0,5.0,"");
    const HistPlotVar& MuonResolution = histPlot->GetNewVar("MuonResolution","MuonResolutions",-1.0,1.0,"");
    const HistPlotVar& Ereco_Egen = histPlot->GetNewVar("Ereco_Egen","#Delta E",-50.0,50.0,"[GeV]");
    const HistPlotVar& E_Z_Parent_Resolution = histPlot->GetNewVar("E_Z_Parent_Resolution","E_{Za}^{#tilde{#chi}_{2a}^{0}}",-200.0,200.0,"[GeV]");
    const HistPlotVar& CosA = histPlot->GetNewVar("CosA","Cos(#theta)",-1.0,1.1,"[Radians]");
    const HistPlotVar& ThetaA = histPlot->GetNewVar("ThetaA","#theta",-5.0,5.0,"[Radians]"); //some angle, see below
    const HistPlotVar& Pull_MET = histPlot->GetNewVar("Pull MET","Pull of MET",-5.0,5.0,"[GeV]");
    const HistPlotVar& DCosA = histPlot->GetNewVar("DCosA","#Delta Cos(#theta)",-0.002,0.002,"[Radians]");
    const HistPlotVar& Pull_Cos = histPlot->GetNewVar("Pull_Cos","Pull of Cos(#theta)",-5.0,5.0,"");
    const HistPlotVar& DElA = histPlot->GetNewVar("DElA","#Delta E_{Lepton}",-25.0,25.0,"[GeV]");
    const HistPlotVar& DBetaA = histPlot->GetNewVar("DBetaA","#Delta #Beta",-0.5,0.5,"[#mu m/pico s]");
    
    const HistPlotVar& ToFaL = histPlot->GetNewVar("ToFaL", "log_{10} ToF(#tilde{#chi}_{2}^{0})", -3.5, 2.5, "[ns]");
    const HistPlotVar& ToFbL = histPlot->GetNewVar("ToFbL", "log_{10} ToF(#tilde{#chi}_{2}^{0})", -3.5, 2.5, "[ns]");
    const HistPlotVar& DaL = histPlot->GetNewVar("DaL", "log_{10} D(#tilde{#chi}_{2}^{0})", -2.5, 4.5, "[cm]");
    const HistPlotVar& DbL = histPlot->GetNewVar("DbL", "log_{10} D(#tilde{#chi}_{2}^{0})", -2.5, 4.5, "[cm]");
    const HistPlotVar& ToFa = histPlot->GetNewVar("ToFa","ToF(#tilde{#chi}_{2}^{0})",0.0,30.0,"[ns]");
    const HistPlotVar& ToFb = histPlot->GetNewVar("ToFb","ToF(#tilde{#chi}_{2}^{0})",0.0,30.0,"[ns]");
    const HistPlotVar& Da = histPlot->GetNewVar("Da", "D(#tilde{#chi}_{2}^{0})", 0.0, 0.05, "[cm]");
    const HistPlotVar& Db = histPlot->GetNewVar("Db", "D(#tilde{#chi}_{2}^{0})", -15.0, 15.0, "[cm]");
    const HistPlotVar& betaa = histPlot->GetNewVar("betaa", "#beta(#tilde{#chi}_{2}^{0})", 0., 1.);
    const HistPlotVar& betab = histPlot->GetNewVar("betab", "#beta(#tilde{#chi}_{2}^{0})", 0., 1.);
    const HistPlotVar& dbeta = histPlot->GetNewVar("dbeta", "(#vec{#beta}^{reco} - #vec{#beta}^{gen}) #bullet #hat{#beta}^{gen}/[1 - #vec{#beta}^{reco}#bullet#vec{#beta}^{gen}]", -1., 1.);
    const HistPlotVar& EZa = histPlot->GetNewVar("EZa", "E_{Za}^{#tilde{#chi}_{2a}^{0}}", 0., 800., "[GeV]");
    const HistPlotVar& Pull_vPta = histPlot->GetNewVar("Pull_vPta","Pull of #delta vPta",-5.0,5.0,"");
    const HistPlotVar& Pull_Mass_Invisible = histPlot->GetNewVar("Pull_Mass_Inv","Pull of M(#tilde{#chi}_{1a}^{0})",-5.0,5.0,"");
    const HistPlotVar& Mreco_Mgen = histPlot->GetNewVar("Mreco_Mgen","#Delta M(#tilde{#chi}_{1a}^{0})",-300.0,300.0,"[GeV]");
    const HistPlotVar& Mass_Invisible_Resolution = histPlot->GetNewVar("Mass_Invisible_Resolution","Invisible Mass Resolution",290.0,310.0,"[GeV]");
    const HistPlotVar& MIa = histPlot->GetNewVar("MIa", "M(#tilde{#chi}_{1a}^{0})", 0., 500., "[GeV]");
    const HistPlotVar& Pull_MXa2 = histPlot->GetNewVar("Pull_MXa2", "Pull of M(#tilde{#chi}_{2a}^{0})", -5.0, 5.0, "");
    const HistPlotVar& Pull_Par = histPlot->GetNewVar("Pull_Par", "Pull of n_{||}", -5.0, 5.0, "");
    const HistPlotVar& Par = histPlot->GetNewVar("Par","n_{||}",0.0,500.0);
    
    const HistPlotVar& EZb = histPlot->GetNewVar("EZb", "E_{Zb}^{#tilde{#chi}_{2b}^{0}}", 0., 800., "[GeV]");
    const HistPlotVar& MXa = histPlot->GetNewVar("MXa", "M(#tilde{#chi}_{2a}^{0})", 0., 1800., "[GeV]");
    const HistPlotVar& MXa2 = histPlot->GetNewVar("MXa2", "M(#tilde{#chi}_{2a}^{0})", 0., 1800., "[GeV]");
    const HistPlotVar& MXb2 = histPlot->GetNewVar("MXb2", "M(#tilde{#chi}_{2b}^{0})", 0., 1800., "[GeV]");
    const HistPlotVar& MIa2 = histPlot->GetNewVar("MIa2", "M(#tilde{#chi}_{1a}^{0})", 0., 1000., "[GeV]");
    const HistPlotVar& RMIXa2 = histPlot->GetNewVar("RMIXa2", "M(#tilde{#chi}_{1a}^{0})/M(#tilde{#chi}_{2a}^{0})", 0., 1.2);
    const HistPlotVar& MXa3 = histPlot->GetNewVar("MXa3", "M(#tilde{#chi}_{2a}^{0})", 0., 2000., "[GeV]");
    const HistPlotVar& MIa3 = histPlot->GetNewVar("MIa3", "M(#tilde{#chi}_{1a}^{0})", 0., 1800., "[GeV]");
    const HistPlotVar& MXb3 = histPlot->GetNewVar("MXb3", "M(#tilde{#chi}_{2b}^{0})", 0., 2000., "[GeV]");
    const HistPlotVar& MIb3 = histPlot->GetNewVar("MIb3", "M(#tilde{#chi}_{1b}^{0})", 0., 1800., "[GeV]");
    
    //comment in/out whatever plots are interesting
    //histPlot->AddPlot(MassReco_MassGen, cat_list);
    //histPlot->AddPlot(Pull_Mass_Parent, cat_list);
    //histPlot->AddPlot(Ereco_Egen, cat_list);
    //histPlot->AddPlot(E_Z_Parent_Resolution, cat_list);
    //histPlot->AddPlot(DElA, cat_list);
    //histPlot->AddPlot(CosA, cat_list);
    //histPlot->AddPlot(DCosA, cat_list);
    //histPlot->AddPlot(Pull_Cos, cat_list);
    //histPlot->AddPlot(ThetaA, cat_list);
    //histPlot->AddPlot(Pull_MET, cat_list);
    //histPlot->AddPlot(DBetaA, cat_list);
    //histPlot->AddPlot(ToFaL, cat_list);
    //histPlot->AddPlot(DaL, cat_list);
    //histPlot->AddPlot(ToFbL, cat_list);
    //histPlot->AddPlot(DbL, cat_list);
    //histPlot->AddPlot(ToFa, cat_list);
    //histPlot->AddPlot(Da, cat_list);
    //histPlot->AddPlot(ToFb, cat_list);
    //histPlot->AddPlot(Db, cat_list);
    //histPlot->AddPlot(betaa, cat_list);
    //histPlot->AddPlot(dbeta, cat_list);
    //histPlot->AddPlot(EZa, cat_list);
    //histPlot->AddPlot(EZb, cat_list);
    //histPlot->AddPlot(MXa, cat_list);
    //histPlot->AddPlot(EZa, EZb, cat_list);
    //histPlot->AddPlot(MXa2, cat_list);
    //histPlot->AddPlot(MIa2, cat_list);
    //histPlot->AddPlot(RMIXa2, cat_list);
    //histPlot->AddPlot(MXa3, cat_list);
    //histPlot->AddPlot(MIa3, cat_list);
    //histPlot->AddPlot(MXa3, MIa3, cat_list);
    //histPlot->AddPlot(MXa3, MXb3, cat_list);
    //histPlot->AddPlot(MIa3, MIb3, cat_list);
    //histPlot->AddPlot(Pull_E_Z, cat_list);
    //histPlot->AddPlot(MuonResolution, cat_list);
    //histPlot->AddPlot(Pull_E_L, cat_list);
    //histPlot->AddPlot(Pull_D, cat_list);
    //histPlot->AddPlot(Pull_T, cat_list);
    //histPlot->AddPlot(Pull_Beta_Mag, cat_list);
    //histPlot->AddPlot(Pull_vPta, cat_list);
    //histPlot->AddPlot(Pull_Mass_Invisible, cat_list);
    //histPlot->AddPlot(Mreco_Mgen, cat_list);
    //histPlot->AddPlot(Mass_Invisible_Resolution, cat_list);
    //histPlot->AddPlot(MIa, cat_list);
    //histPlot->AddPlot(MXa2, cat_list);
    histPlot->AddPlot(Pull_MXa2, cat_list);
    //histPlot->AddPlot(Pull_Par, cat_list);
    //histPlot->AddPlot(Par, cat_list);
    
    TFile* file = new TFile("PTEta.root","READ");
    TH2* hist = (TH2*)file->Get("hist_PTvsEta_1000");
    Physics physics;
    physics.SetEtaPtCM(*hist);
    file->Close();
    delete file;
    
    //build the detector
    Detector PUPPI_Detector;
    //Refer to the formula for MET smearing (see Detector.hh)
    PUPPI_Detector.Set_con0_par(14.7467);
    PUPPI_Detector.Set_con1_par(1.68788);
    PUPPI_Detector.Set_con2_par(-2.95569e-07);
    PUPPI_Detector.Set_con0_perp(15.4667);
    PUPPI_Detector.Set_con1_perp(1.41597);
    PUPPI_Detector.Set_con2_perp(-6.37947e-07);
    PUPPI_Detector.Set_sigmaT(0.03/sqrt(2.)); //timing resolution
    PUPPI_Detector.Set_sigmaPV(20.0/10000.0); //Primary Vertex resolution
    PUPPI_Detector.Set_sigmaSV(65.0/10000.0); //secondary Vertex resolution
    Vertex PV(0.0,0.0,0.0,0.0); //nominal PV at 0
    Resolution test_Resolution(PUPPI_Detector); //seting up the Resolution "calculator"
    double LAB_Pt;
    double LAB_eta;
    
    TVector3 Zhat(0.,0.,1.);
    TVector3 RefVect(1.,1.,1.);
    
    double Epa;
    double Epb;
    double log_10=log(10.);
    
    //relative uncertainty on the distance between PV and SV
    double sigmaDistance = sqrt((PUPPI_Detector.Get_sigmaPV()*PUPPI_Detector.Get_sigmaPV()+PUPPI_Detector.Get_sigmaSV()*PUPPI_Detector.Get_sigmaSV()));
    
    //for checking generator efficiency
    int gen_events = 0;
    int acp_events = 0;
    
  for(int m = 0; m < NmX2; m++){
    g_Log << LogInfo << "Generating events for ";
    g_Log << "mX2 = " << mX2[m] << " , ";
    g_Log << "ctau = " << ctau << LogEnd;
      
      X2a_Gen.SetMass(mX2[m]);
      X2b_Gen.SetMass(mX2[m]);

    LAB_Gen.InitializeAnalysis();
      
    for(int igen = 0; igen < Ngen; igen++){
      if(igen%((std::max(Ngen,10))/10) == 0)
	g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;
        
        // generate event
        LAB_Gen.ClearEvent();                           // clear the gen tree
        physics.GetEtaPtCM(LAB_eta,LAB_Pt);
        LAB_Gen.SetTransverseMomentum(LAB_Pt);
        LAB_Gen.SetLongitudinalMomentum(LAB_Pt*TMath::SinH(LAB_eta));
        
        LAB_Gen.AnalyzeEvent();                         // generate a new event
        gen_events++;
        
        TLorentzVector Pa = X2a_Gen.GetFourVector();
        TLorentzVector Pb = X2b_Gen.GetFourVector();
        TLorentzVector sys = Pa+Pb;
        TLorentzVector Ia  = X1a_Gen.GetFourVector();
        TLorentzVector Ib  = X1b_Gen.GetFourVector();
        TLorentzVector I   = Ia+Ib;
        I.SetZ(0.0);
        
        
        //The smearing begins
        TVector3 MET_RECO_PUPPI = PUPPI_Detector.Smear_MET(sys,I.Vect());
        MET_RECO_PUPPI.SetZ(0.0);
        TVector3 Ia_RECO = PUPPI_Detector.Smear_MET(sys,Ia.Vect());
        Ia_RECO.SetZ(0.0);
        TVector3 Ib_RECO = PUPPI_Detector.Smear_MET(sys,Ib.Vect());
        Ib_RECO.SetZ(0.0);
        TLorentzVector L1a_RECO = PUPPI_Detector.Smear_Muon(L1a_Gen.GetFourVector());
        TLorentzVector L1b_RECO = PUPPI_Detector.Smear_Muon(L1b_Gen.GetFourVector());
        TLorentzVector L2a_RECO = PUPPI_Detector.Smear_Muon(L2a_Gen.GetFourVector());
        TLorentzVector L2b_RECO = PUPPI_Detector.Smear_Muon(L2b_Gen.GetFourVector());
        
        TVector3 vBetaaGen = Pa.BoostVector();
        TVector3 vBetabGen = Pb.BoostVector();
        
        ToFa = physics.Get_ToF(ctau, Pa);
        ToFb = physics.Get_ToF(ctau, Pb);
        
        //ToFa = 1.0e12;
        if(ToFa <= 0.0)
        {
            igen--;
            continue;
        }
        if(ToFb <= 0.0)
        {
            igen--;
            continue;
        }
        
        Vertex SVa = physics.Get_SV(ToFa,Pa);
        Vertex SVb = physics.Get_SV(ToFb,Pb);
        Vertex Smeared_PV = PUPPI_Detector.Smear_PV(PV);
        Vertex Smeared_SVa = PUPPI_Detector.Smear_SV(SVa);
        Vertex Smeared_SVb = PUPPI_Detector.Smear_SV(SVb);
        TVector3 Smeared_vBetaa = PUPPI_Detector.Smear_Beta(Smeared_PV,Smeared_SVa);
        TVector3 Smeared_vBetab = PUPPI_Detector.Smear_Beta(Smeared_PV,Smeared_SVb);
        TVector3 Smeared_vBetaaT = Smeared_vBetaa;
        Smeared_vBetaaT.SetZ(0.0);
        TVector3 Smeared_vBetabT = Smeared_vBetab;
        Smeared_vBetabT.SetZ(0.0);
        TLorentzVector L1a_Gent = L1a_Gen.GetFourVector();
        L1a_Gent.SetZ(0.0);
        TLorentzVector L2a_Gent = L2a_Gen.GetFourVector();
        L2a_Gent.SetZ(0.0);
        TLorentzVector Ia_Gent = Ia;
        Ia_Gent.SetZ(0.0);
        TLorentzVector L1a_RECOt = PUPPI_Detector.Smear_Muon(L1a_Gent);
        TLorentzVector L2a_RECOt = PUPPI_Detector.Smear_Muon(L2a_Gent);
        TLorentzVector L1b_Gent = L1b_Gen.GetFourVector();
        L1b_Gent.SetZ(0.0);
        TLorentzVector L2b_Gent = L2b_Gen.GetFourVector();
        L2b_Gent.SetZ(0.0);
        TLorentzVector Ib_Gent = Ib;
        Ib_Gent.SetZ(0.0);
        TLorentzVector L1b_RECOt = PUPPI_Detector.Smear_Muon(L1b_Gent);
        TLorentzVector L2b_RECOt = PUPPI_Detector.Smear_Muon(L2b_Gent);
        
        
        if(Smeared_vBetaa.Mag() >= 1.)
        {
            igen--;
            continue;
        }
        if(Smeared_vBetab.Mag() >= 1.)
        {
            igen--;
            continue;
        }
        
        betaa = vBetaaGen.Mag();
        betab = vBetabGen.Mag();
        
        Da = 30.*ToFa*betaa;
        Db = 30.*ToFa*betab;
        
        /*
         if(betaa < VelocityUncertainty || betab < VelocityUncertainty) //require significant displacement
         {
         igen--;
         continue;
         }
         */
        
        //set some resolutions
        MuonResolution = PUPPI_Detector.GetMuonResolution(L2a_Gen.GetFourVector());
        double MET_Resolution = PUPPI_Detector.Get_Sigma_Par(sys);
        
        TVector3 vPta = (L1a_RECO+L2a_RECO).Vect()+Ia_RECO;
        TVector3 vPtaGen = (L1a_Gen.GetFourVector()+L2a_Gen.GetFourVector()+Ia).Vect();
        vPtaGen.SetZ(0.0);
        TVector3 vPtb = (L1b_RECO+L2b_RECO).Vect()+Ib_RECO;
        TVector3 vPt  = (L1a_RECO+L2a_RECO+L1b_RECO+L2b_RECO).Vect()+MET_RECO_PUPPI;
        vPta.SetZ(0.);
        vPtb.SetZ(0.);
        vPt.SetZ(0.);
        TVector3 vBetaaT_Gen = vBetaaGen;
        vBetaaT_Gen.SetZ(0.0);
        
        double Smeared_ToFa = PUPPI_Detector.Smear_ToF(ToFa);
        
        double Sigma_Angle = test_Resolution.GetAngleError(vBetaaGen,vPtaGen,Smeared_vBetaa,vPta);
        double Sigma_E_Lepton1 = PUPPI_Detector.GetMuonResolution(L1a_Gen.GetFourVector());
        double Sigma_E_Lepton2 = PUPPI_Detector.GetMuonResolution(L2a_Gen.GetFourVector());
        double Sigma_Angle1 = test_Resolution.GetAngleError(vBetaaGen,L1a_Gen.GetFourVector().Vect(),Smeared_vBetaa,L1a_RECO.Vect());
        double Sigma_Angle2 = test_Resolution.GetAngleError(vBetaaGen,L2a_Gen.GetFourVector().Vect(),Smeared_vBetaa,L2a_RECO.Vect());
        
        //double Sigma_Beta_Mag = sqrt((1.0/(Smeared_ToFa*Smeared_ToFa))*(sigmaDistance*sigmaDistance+2.*Smeared_vBetaa.Dot(vBetaaGen.Unit())*Smeared_vBetaa.Dot(vBetaaGen.Unit())*PUPPI_Detector.Get_sigmaT()*PUPPI_Detector.Get_sigmaT()));
        //double Sigma_Beta_Mag = sqrt((1.0/(Smeared_ToFa*Smeared_ToFa))*(sigmaDistance*sigmaDistance/6.0+2.*Smeared_vBetaa.Mag()*Smeared_vBetaa.Mag()*PUPPI_Detector.Get_sigmaT()*PUPPI_Detector.Get_sigmaT()));
        double Sigma_Beta_Mag = sqrt((1.0/(Smeared_ToFa*Smeared_ToFa))*(sigmaDistance*sigmaDistance+2.*Smeared_vBetaa.Mag()*Smeared_vBetaa.Mag()*PUPPI_Detector.Get_sigmaT()*PUPPI_Detector.Get_sigmaT()));
        
        dbeta = (1./(1.-Smeared_vBetaa.Dot(vBetaaGen)))*(Smeared_vBetaa-vBetaaGen).Dot(vBetaaGen.Unit());
        
        ToFaL = log(ToFa)/log_10;
        DaL   = log(Da)/log_10;
        ToFbL = log(ToFb)/log_10;
        DbL   = log(Db)/log_10;
        
        TVector3 Da_Smear(Smeared_PV.GetXPos()-Smeared_SVa.GetXPos(),Smeared_PV.GetYPos()-Smeared_SVa.GetYPos(),Smeared_PV.GetZPos()-Smeared_SVa.GetZPos());
        TVector3 Da_True(PV.GetXPos()-SVa.GetXPos(),PV.GetYPos()-SVa.GetYPos(),PV.GetZPos()-SVa.GetZPos());
        
        Pull_D = (Da_Smear.Mag()-Da_True.Mag())/sigmaDistance;
        Pull_T = (ToFa-Smeared_ToFa)/PUPPI_Detector.Get_sigmaT();
        //Pull_T = (SVa.GetTPos()-Smeared_SVa.GetTPos())/PUPPI_Detector.Get_sigmaT();
        //check lepton smearing in transverse plane
        Pull_E_L = (L1a_Gent.Pt() - L1a_RECOt.Pt())/(PUPPI_Detector.GetMuonResolution(L1a_Gent)*L1a_RECOt.Pt()); //check lepton smearing in the transverse plane
        
        //Pull_E_L = (L1a_Gen.GetFourVector().E() - L1a_RECO.E())/(PUPPI_Detector.GetMuonResolution(L1a_Gen.GetFourVector())*L1a_RECO.E());
        
        TLorentzVector vZa = L1a_RECO + L2a_RECO;
        vZa.Boost(-Smeared_vBetaa);
        TLorentzVector vZb = L1b_RECO + L2b_RECO;
        vZb.Boost(-Smeared_vBetab);
        
        EZa = vZa.E();
        TLorentzVector vZaGen = L1a_Gen.GetFourVector() + L2a_Gen.GetFourVector();
        vZaGen.Boost(-vBetaaGen);
        
        DElA = L1a_Gen.GetFourVector().E()-L1a_RECO.E();
        Ereco_Egen = EZa - vZaGen.E();
        
        DBetaA = betaa - Smeared_vBetaa.Mag();
        double DBetaAT = vBetaaT_Gen.Mag() - Smeared_vBetaaT.Mag();
        
        Pull_Beta_Mag = DBetaA/Sigma_Beta_Mag;
        
        //Angle Stuff
        
        //test_Resolution.Cos_Resolution(L1a_Gent.Vect(),L2a_Gent.Vect(),PUPPI_Detector.GetMuonResolution(L1a_Gent),PUPPI_Detector.GetMuonResolution(L2a_Gent));
        ThetaA = test_Resolution.GetAngle(L1a_Gen.GetFourVector(),L2a_Gen.GetFourVector());
        
        double Resolution_CosL1aL2a = test_Resolution.Cos_Resolution(L1a_Gen.GetFourVector().Vect(),L2a_Gen.GetFourVector().Vect(),L1a_RECO.E()*PUPPI_Detector.GetMuonResolution(L1a_Gen.GetFourVector()),L2a_RECO.E()*PUPPI_Detector.GetMuonResolution(L2a_Gen.GetFourVector()));
        double Resolution_CosL1aBeta = test_Resolution.Cos_Resolution(L1a_Gen.GetFourVector().Vect(),vBetaaGen,L1a_RECO.E()*PUPPI_Detector.GetMuonResolution(L1a_Gen.GetFourVector()),Sigma_Beta_Mag);
        CosA = TMath::Sin(test_Resolution.GetAngle(L1a_Gen.GetFourVector(),L2a_Gen.GetFourVector()));
        //CosA = TMath::Sin(test_Resolution.GetAngle(L1a_RECO,vBetaaGen));
        //DCosA = CosA - TMath::Cos(test_Resolution.GetAngle(L1a_RECO,L2a_RECO));
        DCosA = TMath::Cos(test_Resolution.GetAngle(L1a_Gen.GetFourVector(),vBetaaGen)) - CosA;
        Pull_Cos = DCosA/Resolution_CosL1aL2a;
        
        
        Pull_MET = (MET_RECO_PUPPI.Mag()-I.Vect().Mag())/MET_Resolution;
        
        
        
        E_Z_Parent_Resolution = test_Resolution.Energy_Z_Parent_Resolution(vBetaaGen,L1a_RECO,L2a_RECO,L1a_RECO.E()*PUPPI_Detector.GetMuonResolution(L1a_Gen.GetFourVector()),L2a_RECO.E()*PUPPI_Detector.GetMuonResolution(L2a_Gen.GetFourVector()),Sigma_Angle1,Sigma_Angle2,Sigma_Beta_Mag); //see Resolution.hh for calculation
        
        //E_Z_Parent_Resolution = test_Resolution.Energy_Z_Parent_Resolution(vBetaaGen,L1a_RECO,L2a_RECO,L1a_RECO.E()*PUPPI_Detector.GetMuonResolution(L1a_Gen.GetFourVector()),L2a_RECO.E()*PUPPI_Detector.GetMuonResolution(L2a_Gen.GetFourVector()),test_Resolution.Cos_Resolution(vBetaaGen,L1a_Gen.GetFourVector().Vect(),Sigma_Beta_Mag,PUPPI_Detector.GetMuonResolution(L1a_Gen.GetFourVector())),test_Resolution.Cos_Resolution(vBetaaGen,L2a_Gen.GetFourVector().Vect(),Sigma_Beta_Mag,PUPPI_Detector.GetMuonResolution(L2a_Gen.GetFourVector())),Sigma_Beta_Mag); //see Resolution.hh for calculation
        
        Pull_E_Z = Ereco_Egen/E_Z_Parent_Resolution; //get the pull of the energy of the Z in the parent's rest frame
        
        
        double MP_Resolution = test_Resolution.Mass_Parent_Resolution(Smeared_vBetaa,Ia_RECO,L1a_RECOt,L2a_RECOt,MET_Resolution,L1a_RECOt.Pt()*PUPPI_Detector.GetMuonResolution(L1a_Gent),L2a_RECOt.Pt()*PUPPI_Detector.GetMuonResolution(L2a_Gent),Sigma_Beta_Mag);
        
        double MXa_Gen = test_Resolution.Mass_Parent(vPtaGen,vBetaaGen);
        MXa = test_Resolution.Mass_Parent(vPta,Smeared_vBetaa);
        MassReco_MassGen = MXa-MXa_Gen;
        Pull_Mass_Parent = MassReco_MassGen/MP_Resolution;
        
        Mass_Invisible_Resolution = test_Resolution.Mass_Invisible_Resolution(Smeared_vBetaa,Ia_RECO,L1a_RECO,L2a_RECO,MET_Resolution,Sigma_Beta_Mag);
        
        double MI_Gen = sqrt(MXa_Gen*MXa_Gen-2.*MXa_Gen*vZaGen.E()+((L1a_Gen.GetFourVector()+L2a_Gen.GetFourVector()).M2()));
        MIa = sqrt(MXa*MXa-2.*MXa*EZa+((L1a_RECO+L2a_RECO).M2()));
        Mreco_Mgen = (MI_Gen-MIa);
        Pull_Mass_Invisible = Mreco_Mgen/Mass_Invisible_Resolution;
        
        TLorentzVector Va = L1a_RECOt+L2a_RECOt;
        TLorentzVector Vb = L1b_RECOt+L2b_RECOt;
        TVector3 Par_Gen = I.Vect()+L1a_Gent.Vect()+L2a_Gent.Vect()+L1b_Gent.Vect()+L2b_Gent.Vect();
        TVector3 Perp_Gen = Par_Gen.Cross(Zhat);
        TVector3 Par_RECO = MET_RECO_PUPPI+Va.Vect()+Vb.Vect();
        TVector3 Perp_RECO = Par_RECO.Cross(Zhat);
        Par = Par_Gen.Mag();//-Par_RECO.Mag();
        /* //for par pull plot
         if(Par < 25.0)
         {
         igen--;
         continue;
         }
         */
        double Sigma_Par = test_Resolution.Par_Resolution(MET_RECO_PUPPI,L1a_RECOt.Vect(),L2a_RECOt.Vect(),L1b_RECOt.Vect(),L2b_RECOt.Vect(),MET_Resolution,L1a_RECOt.Pt()*PUPPI_Detector.GetMuonResolution(L1a_Gent),L2a_RECOt.Pt()*PUPPI_Detector.GetMuonResolution(L2a_Gent),L1b_RECOt.Pt()*PUPPI_Detector.GetMuonResolution(L1b_Gent),L2b_RECOt.Pt()*PUPPI_Detector.GetMuonResolution(L2b_Gent));
        
        Pull_Par = (Par_Gen.Mag()-Par_RECO.Mag())/Sigma_Par;
        
        double gammaa_Gen = 1./sqrt(1.-vBetaaGen.Mag2());
        double gammab_Gen = 1./sqrt(1.-vBetabGen.Mag2());
        double gammaa_RECO = 1./sqrt(1.-Smeared_vBetaa.Mag2());
        double gammab_RECO = 1./sqrt(1.-Smeared_vBetab.Mag2());
        
        double MPa_Gen = test_Resolution.Mass_Parents2(Par_Gen,vBetaaGen,vBetabGen);
        MXa2 = test_Resolution.Mass_Parents2(Par_RECO,Smeared_vBetaa,Smeared_vBetab);
        
        Pull_MXa2 = (MPa_Gen-MXa2)/test_Resolution.Mass_Parents2_Resolution(Par_RECO,Smeared_vBetaa,Smeared_vBetab,Sigma_Beta_Mag,Sigma_Par);
        /*
         EZb = vZb.E();
         MXa = EZa + sqrt(EZa*EZa + mX1*mX1 - vZa.M2());
         
         MXa2 = vPta.Dot(Smeared_vBetaaT)*sqrt(1.-Smeared_vBetaa.Mag2())/Smeared_vBetaaT.Mag2();
         MXa2 = vPtb.Dot(Smeared_vBetabT)*sqrt(1.-Smeared_vBetab.Mag2())/Smeared_vBetabT.Mag2();
         MIa2 = sqrt(MXa2*MXa2 - 2.*MXa2*EZa+vZa.M2());
         RMIXa2 = MIa2 / MXa2;
         
         MXa3 = (vZa.M2()-vZb.M2())/2./(EZa-EZb);
         MIa3 = sqrt(MXa3*MXa3 - 2.*MXa3*EZa+vZa.M2());
         
         gammaa = 1./sqrt(1.-Smeared_vBetaa.Mag2());
         gammab = 1./sqrt(1.-Smeared_vBetab.Mag2());
         
         TVector3 vPar = vPt.Unit();
         TVector3 vPer = vPt.Cross(Zhat).Unit();
         
         BaPar = Smeared_vBetaaT.Dot(vPar);
         BaPer = Smeared_vBetaaT.Dot(vPer);
         BbPar = Smeared_vBetabT.Dot(vPar);
         BbPer = Smeared_vBetabT.Dot(vPer);
         
         Epa = vPt.Mag()/( BaPar - BbPar*BaPer/BbPer );
         MXa3 = Epa/gammaa;
         Epb = vPt.Mag()/( BbPar - BaPar*BbPer/BaPer );
         MXb3 = Epb/gammab;
         MIb3 = sqrt(MXb3*MXb3 - 2.*MXb3*EZb+vZb.M2());
         */
        histPlot->Fill(cat_list[m]);
        acp_events++;
    }
      LAB_Gen.PrintGeneratorEfficiency();
  }
    histPlot->Draw();
    
    TFile fout(output_name.c_str(),"RECREATE");
    fout.Close();
    histPlot->WriteOutput(output_name);
    histPlot->WriteHist(output_name);
    treePlot->WriteOutput(output_name);
    
    g_Log << LogInfo << "Finished" << LogEnd;
    g_Log << LogInfo << "Generated a Total of " << gen_events << " Events " << LogEnd;
    g_Log << LogInfo << acp_events << " passed selection requirements " << LogEnd;
    g_Log << LogInfo << "Efficiency: " << 100.0*acp_events/gen_events << "%" << LogEnd;
    Long64_t end = gSystem->Now();
    g_Log << LogInfo << "Time to Generate " << Ngen*NmX2 << " Events: " << (end-start)/1000.0 << " seconds" << LogEnd;
}
