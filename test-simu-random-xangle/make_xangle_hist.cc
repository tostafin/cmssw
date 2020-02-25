{
  TH1D *h_xangle = new TH1D("h_xangle", ";xangle (#murad)", 4, 115., 155.);
  h_xangle->Fill(120., 1.);
  h_xangle->Fill(140., 2.);
  h_xangle->Fill(150., 1.);
  
  TFile *f_out = TFile::Open("xangle_hist.root", "recreate");
  h_xangle->Write();
  delete f_out;
}
