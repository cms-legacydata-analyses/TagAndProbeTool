#include "../src/get_2d_array.cpp"

void test_3d_to_2d()
{
	double array3d[3][2][4] = {
		{
			{1., 2., 3., 4.},
			{1., 2., 3., 4.}
		},
		{
			{1., 2., 3., 4.},
			{1., 2., 3., 4.}
		},
		{
			{1., 2., 3., 4.},
			{1., 2., 3., 4.}
		},
	};

	double** array2d = get_2d_array(array3d, 0);

	int size1 = sizeof(array3d)/sizeof(*array3d);
	int size2 = sizeof(array3d[0])/sizeof(*array3d[0]);
	for (int i = 0; i < size1-1; i++)
	{
		for (int j = 0; j < size2-1; j++)
		{
			printf("(%d, %d) = %f", i, j, array2d[i][j]);
		}
	}
}