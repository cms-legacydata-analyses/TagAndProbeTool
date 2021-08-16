void create_folder(const char* folderPath, bool deleteOld = false)
{
	//Delete old directory for saving files
	if (deleteOld == true)
		gSystem->Exec((string("rm -r ") + string(folderPath) + string("*")).c_str());

	//Check if dir exists and create
	if (gSystem->AccessPathName(folderPath))
		if (gSystem->mkdir(folderPath, true))
		{
			cerr << "\"" << folderPath << "\" path could not be found and could not be created ERROR\n";
			cerr << "Try to create manually this folder path\n";
			abort();
		}
		else
			cout << "\"" << folderPath << "\" directory created OK\n";
	else
		cout << "\"" << folderPath << "\" directory OK\n";
}