// Ȯ�������� ������Ʈ #1
// 7�� 
// 12164724������  12181748��ҿ�  12181771�ΰ��  12181850������
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <cmath>
#define INF 10000
#define M_INF -10000
#define PI 3.141592
#define e 2.718281
#define PDF_RANGE 100  // N, Y�� PDF �׷����� ��� ���� ���� ���� ����
#define DATA_NUM 10000 // �����ϴ� �������� ����. Ŭ���� �׷����� ��Ȯ������.

using namespace std;

template<typename E>
E MAX(E a, E b) { // a �� b �߿� �� ū ���� ����ϴ� �Լ�
	return (a < b ? b : a);
}

template<typename E>
E MIN(E a, E b) { // a�� b �߿� �� ���� ���� ����ϴ� �Լ�
	return (a < b ? a : b);
}

double Gaussian(double x, double avg, double var) { // Gaussian function
	double left = 1.0 / (sqrt(2.0 * PI) * var);
	double right = pow(e, (-1.0) * pow(x + avg, 2.0) / (2 * pow(var, 2.0)));
	double ans = right * left;
	return ans;
}

double Gaussian_Integral(double p, double avg, double var, double from, double to) {
	double gap;
	gap = 0.001; // ���� ���� ����ϱ� ���� dx�� ��(�簢���� ���α���)�̴�.
	double sum = 0; // return�� ���� ��� ��

	vector<double> function; // PDF���� �����ϱ� ���� vector

	FILE* functionForm;			// Gaussian PDF ���� �� ����
	FILE* indexOfFunctionForm;	// Gaussian PDF�� �� ��ġ�� index�� �� ����
	fopen_s(&functionForm, "functionForm.txt", "w"); // Gaussian PDF ������ text file�� ������ �ȴ�.
	fopen_s(&indexOfFunctionForm, "indexOfFunctionForm.txt", "w"); // Gaussian PDF�� �� ��ġ�� index��
																   // text file�� ������ �ȴ�.

	int curIndex = 0; // for�� ������ �۾��� function vector�� index ��

	for (double cur = from; cur <= to; cur += gap) {
		double value = p * Gaussian(cur, avg, var); // ���� PDF�� ���� ����þ� ����
													// Ȯ�� p���� ���� ���̴�.
		if (value) {
			function.push_back(value); // function vector�� ���� Gaussian ���� �ִ´�.

			if (functionForm) fprintf(functionForm, "%f\n", function[curIndex]);
			if (indexOfFunctionForm) fprintf(indexOfFunctionForm, "%f\n", cur);

			sum += function[curIndex] * gap; // ����ؼ� ���� �簢���� ���̸� ���Ѵ�.

			curIndex++; // ���� for�� ������ �۾����� function vector�� index�� �� ����
		}
	}

	if (functionForm) fclose(functionForm);
	if (indexOfFunctionForm) fclose(indexOfFunctionForm);

	return sum; // from���� to���� ��� �� ���а��� return�Ѵ�.
}

void main() {
	double random_number, number;

	FILE* Y_pdf_file;   //  Y_pdf ������ �� ����
	FILE* Y_index_file; //  Y_index ������ �� ����
	FILE* N_pdf_file;   //  N_pdf ������ �� ����
	fopen_s(&Y_pdf_file, "Y_pdf_file.txt", "w");     //  Y_pdf ������ text file�� ������ �ȴ�.
	fopen_s(&Y_index_file, "Y_index_file.txt", "w"); //  Y_index ������ text file�� ������ �ȴ�.
	fopen_s(&N_pdf_file, "N_pdf_file.txt", "w");     //  N_pdf ������ text file�� ������ �ȴ�.

	double* X = new double[DATA_NUM * 2]; // Sender�� ���� ��ȣ ������ �����ϴ� �迭
	double* N = new double[DATA_NUM * 2]; // Noise������ �����ϴ� �迭
	double* Y = new double[DATA_NUM * 2]; // Reciever�� ���� ��ȣ ������ �����ϴ� �迭

	double* Y_pdf = new double[PDF_RANGE * 2];   //  Y[i]�� ������ ������ ������ ������ �� ������ �ش��� �Ǵ� �͵��� Ȯ��
	double* Y_index = new double[PDF_RANGE * 2]; //  �׷��� �󿡼� x���� ������ ���� ���̴�. Y_pdf[i]�� x���� ���� Y_index[i]�� �ȴ�.
	double* N_pdf = new double[PDF_RANGE * 2];   //  N[i]�� ������ ������ ������ ������ �� ������ �ش��� �Ǵ� �͵��� Ȯ��

	// X, Y, N, Y_pdf, Y_index, N_pdf �� �ʿ��� �������� 2�� �� ���� �޸� ������ �������� �Ҵ� �� ������
	// �ʿ��� ���� ��ŭ�� �޸� ������ �Ͽ��� �� �Ʒ� 166, 167���� ���� '+='�� ���� ���ڿ� ����
	// �����ڸ� �����ε� �� ��� ���� ������ ������ ������ �ٸ��Ƿ� ���� delete[]�������� ������ �߻��ϱ� �����̴�.

	double v = 0.5;        //  Gaussian function���� ����� �� ��հ��̴�.
	double minus_v = (-1) * v; //  Gaussian function���� ����� �� ��հ��̴�.
	double var = 1;        //  Gaussian function���� ����� �� �л갪�̴�.
	double p = 0.5;        //  Gaussian function���� ����� �� ��հ��̴�.

	for (int i = 0; i < PDF_RANGE; i++) {
		Y_pdf[i] = 0; //  ���� ������ Y_pdf[i]�� ���� 1�� ������Ű�� �۾��� 0���� ���� �ʱ�ȭ �Ͽ���.
		N_pdf[i] = 0; //  ���� ������ N_pdf[i]�� ���� 1�� ������Ű�� �۾��� 0���� ���� �ʱ�ȭ �Ͽ���.
	}

	double Y_min = INF; //  Y �� �ּҰ� (���� �񱳵� ������ ������������ Ŭ ���� INF�� �����Ͽ���.)
	double Y_max = -1;  //  Y �� �ִ밪 (���� �񱳵� ������ ������������ ���� ���� -1�� �����Ͽ���.)
	double N_min = INF; //  N �� �ּҰ� (���� �񱳵� ������ ������������ Ŭ ���� INF�� �����Ͽ���.)
	double N_max = -1;  //  N �� �ִ밪 (���� �񱳵� ������ ������������ ���� ���� -1�� �����Ͽ���.)

	double v1, v2, s;             //  Noise�� ���� �����ϴ� ���� ����� �� �������̴�.
	int R0 = 0;     int R1 = 0;   //  0�� ���� Ƚ��, 1�� ���� Ƚ���� ������ �������̴�.
	int S0R1 = 0;   int S1R0 = 0; //  0�� �������� 1�� ���� Ƚ��, 1�� �������� 0�� ���� Ƚ���� ������ �������̴�.

	srand((unsigned)time(NULL));

	for (int i = 0; i < DATA_NUM; i++) { //  DATA_NUM ���� random data�� �����Ѵ�.
		random_number = rand();            // random_number�� ���� 0 ~ 32,767 ������ ���������� �ʱ�ȭ�� �Ѵ�.
		number = random_number / RAND_MAX; //  RAND_MAX�� ������ �����ν� number �� 0 ~ 1 ������ ���� �ȴ�. ( RAND_MAX = 32,767 )

		if (number > (1 - p)) // �߻��� ���� (1 - p)���� ū ���
			X[i] = v;
		else                  // �߻��� ���� (1 - p)������ ���
			X[i] = minus_v;

		//  ******************* 3�� *******************
		do { // Q - Q plot �̿��Ͽ� ����þ� ���� ��Ÿ����
			v1 = 2 * ((double)rand() / RAND_MAX) - 1; //  -1 <= v1 <= 1
			v2 = 2 * ((double)rand() / RAND_MAX) - 1; //  -1 <= v2 <= 1

			s = v1 * v1 + v2 * v2;      //uniform �������� ������ s, �������� �������� ��ĥ ���� ����þ� ������ ������.
		} while (s >= 1 || s == 0);     //  ����� 0�̰ų�, ǥ�������� 1�϶� ����
		s = sqrt((-2 * log(s)) / s);    //  ���Ժ����� �� ��������� ����

		N[i] = v1 * s;					//  ������� ����þ� ������ ����

		Y[i] = X[i] + N[i]; //  ******************* 4�� *******************

		Y_max = MAX(Y_max, Y[i]);  //  ����ؼ� max value of Y �� ������Ʈ �Ѵ�.
		Y_min = MIN(Y_min, Y[i]);  //  ����ؼ� min value of Y �� ������Ʈ �Ѵ�.
		N_max = MAX(N_max, N[i]);  //  ����ؼ� max value of N �� ������Ʈ �Ѵ�.
		N_min = MIN(N_min, N[i]);  //  ����ؼ� min value of N �� ������Ʈ �Ѵ�.

		if (Y[i] < 0) {            //  0�� �޾Ҵٰ� �Ǵ� �� ���
			R0++;                  //    0�� �޾����Ƿ� R0�� 1 ������Ų��.
			if (X[i] > 0)          //    0�� ���� �����ε� 1�� ���� ����� 
				S1R0++;            //    S1R0�� ������Ų��.
		}
		else if (Y[i] >= 0) {      //  1�� �޾Ҵٰ� �Ǵ� �� ���
			R1++;                  //    1�� �޾����Ƿ� R1�� 1 ������Ų��.    
			if (X[i] <= 0)         //    1�� ���� �����ε� 0�� ���� ����� 
				S0R1++;            //    S0R1�� ������Ų��.
		}
	}

	double Y_gap = (Y_max - Y_min) / PDF_RANGE; //  Y�� pdf�� ���ϴ� ������ ��
	double N_gap = (N_max - N_min) / PDF_RANGE; //  N�� pdf�� ���ϴ� ������ ��

	for (int i = 0; i < DATA_NUM; i++) {
		int Y_index = (Y[i] - Y_min) / Y_gap; //  �� Y[i]������ �ش�Ǵ� index�� ���� ���Ѵ�.
		Y_pdf[Y_index] += 1.0;                //  �� index�� �ش�Ǵ� Y_pdf�� 1 ������Ų��.

		int N_index = (N[i] - N_min) / N_gap; //  �� N[i]������ �ش�Ǵ� index�� ���� ���Ѵ�.
		N_pdf[N_index] += 1.0;                //  �� index�� �ش�Ǵ� N_pdf�� 1 ������Ų��.
	}

	Y_pdf[PDF_RANGE - 1] += 1.0; // �� for������ Y_max�� ���� ���� ó���� ���� �����Ƿ� ���� ó���� �� �־���.
	N_pdf[PDF_RANGE - 1] += 1.0; // �� for������ N_max�� ���� ���� ó���� ���� �����Ƿ� ���� ó���� �� �־���.

	double Sum_of_Percentage = 0; // Y�� pdf���� ��� ���� �����̴�. ���������� 1�� �Ǿ�� �Ѵ�.

	for (int i = 0; i < PDF_RANGE; i++) {
		Y_pdf[i] /= DATA_NUM;  //  ���� Y_pdf[i]���� �� ������ �ش�Ǵ� Y�� ������ ���ִ�.
							   //    ���� �� DATA������ ������ Ȯ���� �ȴ�.
		N_pdf[i] /= DATA_NUM;  //  ���� N_pdf[i]���� �� ������ �ش�Ǵ� N�� ������ ���ִ�.
							   //    ���� �� DATA������ ������ Ȯ���� �ȴ�.
	}

	for (int i = 0; i < PDF_RANGE; i++) {
		Y_index[i] = Y_min + i * Y_gap; //  �׷������� x���� ���� �ش� ������ �ּҰ����� �Ѵ�.
		Sum_of_Percentage += Y_pdf[i];  //  Y�� pdf���� ���ϴ� �����̴�.

		fprintf(Y_pdf_file, "%f\n", Y_pdf[i]);      // Y_pdf[i]�� ���� ������ open�� Y_pdf_file.txt�� �����Ѵ�.
		fprintf(Y_index_file, "%f\n", Y_index[i]);  // Y_index[i]�� ���� ������ open�� Y_index_file.txt�� �����Ѵ�.

		fprintf(N_pdf_file, "%f\n", N_pdf[i]);      // N_pdf[i]�� ���� ������ open�� N_pdf_file.txt�� �����Ѵ�.
	}

	cout << " Y�� pdf�� �� �� : " << Sum_of_Percentage << '\n' << '\n'; // Y�� pdf ���� ��� ���� ���� ����Ѵ�.
	cout << " * ���� v �� �� : " << v << '\n';
	cout << " * ���� p �� �� : " << p << '\n';

	// ******************* 6�� *******************
	cout << '\n';
	cout << " ********** 6�� **********" << '\n';
	cout << " Noise ���� �� 0�̶� �Ǵ� : " << R0 << '\n';
	cout << " Noise ���� �� 1�̶� �Ǵ� : " << R1 << '\n';

	// ******************* 7�� *******************
	cout << '\n';
	cout << " ********** 7�� **********" << '\n';
	cout << " 0�� ���´µ� 1�� ���� Ƚ�� : " << S0R1 << '\n';
	cout << " 1�� ���´µ� 0�� ���� Ƚ�� : " << S1R0 << '\n' << '\n';

	cout << " 0�� ���´µ� 1�� �޾��� Ȯ�� : " << double(S0R1) / double(DATA_NUM) << '\n'; // �̰� m, a �� ������ �˾ƾ� ��.
	cout << " 1�� ���´µ� 0�� �޾��� Ȯ�� : " << double(S1R0) / double(DATA_NUM) << '\n'; // �̰� m, a �� ������ �˾ƾ� ��.

	// ******************* 8�� *******************
	cout << '\n';
	cout << " ********** 8�� **********" << '\n';
	cout << " 0�� ���´µ� 1���� ���� Ȯ�� : " << Gaussian_Integral(1 - p, v, var, 0, INF) << '\n';
	cout << " 1�� ���´µ� 0���� ���� Ȯ�� : " << Gaussian_Integral(p, minus_v, var, M_INF, 0) << '\n';

	/*   cout << '\n';
	cout << " ********** 9�� **********" << '\n';
	cout << " 0�� ���´µ� 1���� ���� Ȯ�� : " << Gaussian_Integral(1 - p, v, var, 0, INF) << '\n';
	cout << " 1�� ���´µ� 0���� ���� Ȯ�� : " << Gaussian_Integral(p, minus_v, var, M_INF, 0) << '\n';*/

	//cout << '\n';
	//cout << " ********** 10�� **********" << '\n';
	//cout << " 0�� ���´µ� 1���� ���� Ȯ�� : " << Gaussian_Integral(1 - p, v, var, 0, INF) << '\n';
	//cout << " 1�� ���´µ� 0���� ���� Ȯ�� : " << Gaussian_Integral(p, minus_v, var, M_INF, 0) << '\n';

	fclose(Y_pdf_file);
	fclose(Y_index_file);
	fclose(N_pdf_file);

	delete[] X;
	delete[] Y;
	delete[] N;
	delete[] Y_pdf;
	delete[] Y_index;
	delete[] N_pdf;
}