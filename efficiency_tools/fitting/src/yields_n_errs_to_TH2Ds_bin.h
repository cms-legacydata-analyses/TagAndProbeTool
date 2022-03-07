
void yields_n_errs_to_TH2Ds_bin(TH2D* hist2d_all, TH2D* hist2d_pass, int x, int y, double* yields_n_errs)
{
	hist2d_all ->SetBinContent(x, y, yields_n_errs[0]);
	hist2d_pass->SetBinContent(x, y, yields_n_errs[1]);
	hist2d_all ->SetBinError  (x, y, yields_n_errs[2]);
	hist2d_pass->SetBinError  (x, y, yields_n_errs[3]);
}