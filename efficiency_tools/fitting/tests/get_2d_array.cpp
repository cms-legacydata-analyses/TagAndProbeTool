double** get_2d_array(double*** array3d, int last_index)
{
	const int size1 = sizeof(array3d)/sizeof(*array3d);
	const int size2 = sizeof(array3d[0])/sizeof(*array3d[0]);

	double** array2d  = new double*[size1];
	
	for (int i = 0; i < size1-1; i++)
	{
		double* row = new double[size2];
		for (int j = 0; j < size2-1; j++)
		{
			row[j] = array3d[i][j][last_index];
		}
		array2d[i] = row;
	}

	return array2d;
}