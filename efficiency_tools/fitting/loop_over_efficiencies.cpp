#include "efficiency.cpp"
double default_min = _mmin;
double default_max = _mmax;

bool should_loop_muon_id  = false;
bool should_loop_settings = false;
int  setting = 2;

bool exactly = false;

void set_settings(int index, bool exactly = false)
{
	string min_string = "";
	string max_string = "";
	switch(index)
	{
		case 0:
			prefix_file_name = "nominal_";
			break;
		case 1:
			_mmin = default_min - 0.05;
			_mmax = default_max + 0.05;
			min_string = to_string(_mmin);
			max_string = to_string(_mmax);
			if (exactly)
			{
				replace(min_string.begin(), min_string.end(), '.', 'p');
				replace(max_string.begin(), max_string.end(), '.', 'p');
				prefix_file_name  = string("mass_") + min_string.substr(0, min_string.length()-4) + string("_");
				prefix_file_name +=                   max_string.substr(0, max_string.length()-4) + string("_");
			}
			else
				prefix_file_name = "MassUp_";
			break;
		case 2:
			_mmin = default_min + 0.05;
			_mmax = default_max - 0.05;
			min_string = to_string(_mmin);
			max_string = to_string(_mmax);
			if (exactly)
			{
				replace(min_string.begin(), min_string.end(), '.', 'p');
				replace(max_string.begin(), max_string.end(), '.', 'p');
				prefix_file_name  = string("mass_") + min_string.substr(0, min_string.length()-4) + string("_");
				prefix_file_name +=                   max_string.substr(0, max_string.length()-4) + string("_");
			}
			else
				prefix_file_name = "MassDown_";
			break;
		case 3:
			fit_bins = 95;
			if (exactly)
				prefix_file_name = string("binfit") + to_string(fit_bins) + string("_");
			else
				prefix_file_name = "binDown_";
			break;
		case 4:
			fit_bins = 105;
			if (exactly)
				prefix_file_name = string("binfit") + to_string(fit_bins) + string("_");
			else
				prefix_file_name = "binDown_";
			break;
	}
}

void loop_settings()
{
	for (int i = 0; i <= 4; i++)
	{
		set_settings(i, exactly);
		efficiency();
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
			efficiency();
	}
}

void loop_over_efficiencies()
{
	if(setting >= 0)
		set_settings(setting, exactly);

	if (should_loop_muon_id)
		loop_muon_id();
	else if (should_loop_settings)
		loop_settings();
	else
		efficiency();
}