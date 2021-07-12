#include "src/efficiency.cpp"

void loop_settings()
{
	string min_string = "";
	string max_string = "";
	for (int i = 0; i <= 4; i++)
	{
		switch(i)
		{
			case 0:
				prefix_file_name = "nominal_";
				break;
			case 1:
				_mmin = default_min - 0.05;
				_mmax = default_max + 0.05;
				min_string = to_string(_mmin);
				max_string = to_string(_mmax);
				replace(min_string.begin(), min_string.end(), '.', 'p');
				replace(max_string.begin(), max_string.end(), '.', 'p');
				prefix_file_name  = string("mass_") + min_string.substr(0, min_string.length()-4) + string("_");
				prefix_file_name +=                   max_string.substr(0, max_string.length()-4) + string("_");
				break;
			case 2:
				_mmin = default_min + 0.05;
				_mmax = default_max - 0.05;	
				min_string = to_string(_mmin);
				max_string = to_string(_mmax);
				replace(min_string.begin(), min_string.end(), '.', 'p');
				replace(max_string.begin(), max_string.end(), '.', 'p');
				prefix_file_name  = string("mass_") + min_string.substr(0, min_string.length()-4) + string("_");
				prefix_file_name +=                   max_string.substr(0, max_string.length()-4) + string("_");
				break;
			case 3:
				fit_bins = 95;
				prefix_file_name = "binfit95_";
				break;
			case 4:
				fit_bins = 105;
				prefix_file_name = "binfit105_";
				break;
		}
		efficiency_calculus();
	}
}

void loop_muon_id()
{
	for (int i = 0; i <= 2; i++)
	{
		switch(i)
		{
			case 0:
				MuonId   = "trackerMuon";
				break;
			case 1:
				MuonId   = "standaloneMuon";
				break;
			case 2:
				MuonId   = "globalMuon";
				break;
		}		

		if (should_loop_settings)
			loop_settings();
		else
			efficiency_calculus();
	}
}

void sys_efficiency()
{
	if (should_loop_muon_id)
		loop_muon_id();
	else if (should_loop_settings)
		loop_settings();
	else
		efficiency_calculus();
}