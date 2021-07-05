// 확률변수론 프로젝트 #1
// 7조 
// 12164724최조훈  12181748김소원  12181771두경빈  12181850허유진
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
#define PDF_RANGE 100  // N, Y의 PDF 그래프를 얻기 위한 범위 분할 개수
#define DATA_NUM 10000 // 참고하는 데이터의 개수. 클수록 그래프가 명확해진다.

using namespace std;

template<typename E>
E MAX(E a, E b) { // a 와 b 중에 더 큰 값을 출력하는 함수
	return (a < b ? b : a);
}

template<typename E>
E MIN(E a, E b) { // a와 b 중에 더 작은 값을 출력하는 함수
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
	gap = 0.001; // 적분 값을 계산하기 위한 dx의 값(사각형의 가로길이)이다.
	double sum = 0; // return할 적분 계산 값

	vector<double> function; // PDF값을 저장하기 위한 vector

	FILE* functionForm;			// Gaussian PDF 값이 들어갈 파일
	FILE* indexOfFunctionForm;	// Gaussian PDF의 각 위치별 index가 들어갈 파일
	fopen_s(&functionForm, "functionForm.txt", "w"); // Gaussian PDF 값들이 text file로 저장이 된다.
	fopen_s(&indexOfFunctionForm, "indexOfFunctionForm.txt", "w"); // Gaussian PDF의 각 위치별 index가
																   // text file로 저장이 된다.

	int curIndex = 0; // for문 내에서 작업할 function vector의 index 값

	for (double cur = from; cur <= to; cur += gap) {
		double value = p * Gaussian(cur, avg, var); // 현재 PDF의 값은 가우시안 값에
													// 확률 p값을 곱한 값이다.
		if (value) {
			function.push_back(value); // function vector에 현재 Gaussian 값을 넣는다.

			if (functionForm) fprintf(functionForm, "%f\n", function[curIndex]);
			if (indexOfFunctionForm) fprintf(indexOfFunctionForm, "%f\n", cur);

			sum += function[curIndex] * gap; // 계속해서 현재 사각형의 넓이를 더한다.

			curIndex++; // 현재 for문 내에서 작업중인 function vector의 index의 값 증가
		}
	}

	if (functionForm) fclose(functionForm);
	if (indexOfFunctionForm) fclose(indexOfFunctionForm);

	return sum; // from부터 to까지 계산 된 적분값을 return한다.
}

void main() {
	double random_number, number;

	FILE* Y_pdf_file;   //  Y_pdf 값들이 들어갈 파일
	FILE* Y_index_file; //  Y_index 값들이 들어갈 파일
	FILE* N_pdf_file;   //  N_pdf 값들이 들어갈 파일
	fopen_s(&Y_pdf_file, "Y_pdf_file.txt", "w");     //  Y_pdf 값들이 text file로 저장이 된다.
	fopen_s(&Y_index_file, "Y_index_file.txt", "w"); //  Y_index 값들이 text file로 저장이 된다.
	fopen_s(&N_pdf_file, "N_pdf_file.txt", "w");     //  N_pdf 값들이 text file로 저장이 된다.

	double* X = new double[DATA_NUM * 2]; // Sender가 보낸 신호 값들을 저장하는 배열
	double* N = new double[DATA_NUM * 2]; // Noise값들을 저장하는 배열
	double* Y = new double[DATA_NUM * 2]; // Reciever가 받은 신호 값들을 저장하는 배열

	double* Y_pdf = new double[PDF_RANGE * 2];   //  Y[i]의 값들을 임의의 범위를 나누어 그 범위에 해당이 되는 것들의 확률
	double* Y_index = new double[PDF_RANGE * 2]; //  그래프 상에서 x축의 값으로 사용될 값이다. Y_pdf[i]의 x축의 값은 Y_index[i]가 된다.
	double* N_pdf = new double[PDF_RANGE * 2];   //  N[i]의 값들을 임의의 범위를 나누어 그 범위에 해당이 되는 것들의 확률

	// X, Y, N, Y_pdf, Y_index, N_pdf 이 필요한 공간보다 2배 더 많은 메모리 공간을 동적으로 할당 한 이유는
	// 필요한 공간 만큼만 메모리 선언을 하였을 때 아래 166, 167번과 같이 '+='와 같은 문자열 연결
	// 연산자를 오버로딩 한 경우 사용된 공간이 선언한 공간과 다르므로 이후 delete[]과정에서 오류가 발생하기 때문이다.

	double v = 0.5;        //  Gaussian function에서 사용이 될 평균값이다.
	double minus_v = (-1) * v; //  Gaussian function에서 사용이 될 평균값이다.
	double var = 1;        //  Gaussian function에서 사용이 될 분산값이다.
	double p = 0.5;        //  Gaussian function에서 사용이 될 평균값이다.

	for (int i = 0; i < PDF_RANGE; i++) {
		Y_pdf[i] = 0; //  이후 임의의 Y_pdf[i]의 값을 1씩 증가시키는 작업을 0으로 전부 초기화 하였다.
		N_pdf[i] = 0; //  이후 임의의 N_pdf[i]의 값을 1씩 증가시키는 작업을 0으로 전부 초기화 하였다.
	}

	double Y_min = INF; //  Y 의 최소값 (이후 비교될 값보다 무조건적으로 클 값인 INF로 정의하였다.)
	double Y_max = -1;  //  Y 의 최대값 (이후 비교될 값보다 무조건적으로 작을 값인 -1로 정의하였다.)
	double N_min = INF; //  N 의 최소값 (이후 비교될 값보다 무조건적으로 클 값인 INF로 정의하였다.)
	double N_max = -1;  //  N 의 최대값 (이후 비교될 값보다 무조건적으로 작을 값인 -1로 정의하였다.)

	double v1, v2, s;             //  Noise의 값을 정의하는 데에 사용이 될 변수들이다.
	int R0 = 0;     int R1 = 0;   //  0을 받은 횟수, 1을 받은 횟수를 저장할 변수들이다.
	int S0R1 = 0;   int S1R0 = 0; //  0을 보냈지만 1을 받은 횟수, 1을 보냈지만 0을 받은 횟수를 저장할 변수들이다.

	srand((unsigned)time(NULL));

	for (int i = 0; i < DATA_NUM; i++) { //  DATA_NUM 개의 random data를 생성한다.
		random_number = rand();            // random_number의 값을 0 ~ 32,767 사이의 랜덤값으로 초기화를 한다.
		number = random_number / RAND_MAX; //  RAND_MAX로 나누어 줌으로써 number 는 0 ~ 1 사이의 수가 된다. ( RAND_MAX = 32,767 )

		if (number > (1 - p)) // 발생된 수가 (1 - p)보다 큰 경우
			X[i] = v;
		else                  // 발생된 수가 (1 - p)이하인 경우
			X[i] = minus_v;

		//  ******************* 3번 *******************
		do { // Q - Q plot 이용하여 가우시안 분포 나타내기
			v1 = 2 * ((double)rand() / RAND_MAX) - 1; //  -1 <= v1 <= 1
			v2 = 2 * ((double)rand() / RAND_MAX) - 1; //  -1 <= v2 <= 1

			s = v1 * v1 + v2 * v2;      //uniform 분포에서 임의의 s, 독립분포 여러개를 합칠 수록 가우시안 분포를 따른다.
		} while (s >= 1 || s == 0);     //  평균이 0이거나, 표준편차가 1일때 적용
		s = sqrt((-2 * log(s)) / s);    //  정규분포와 더 가까워지기 위해

		N[i] = v1 * s;					//  노이즈는 가우시안 분포를 가짐

		Y[i] = X[i] + N[i]; //  ******************* 4번 *******************

		Y_max = MAX(Y_max, Y[i]);  //  계속해서 max value of Y 를 업데이트 한다.
		Y_min = MIN(Y_min, Y[i]);  //  계속해서 min value of Y 를 업데이트 한다.
		N_max = MAX(N_max, N[i]);  //  계속해서 max value of N 을 업데이트 한다.
		N_min = MIN(N_min, N[i]);  //  계속해서 min value of N 을 업데이트 한다.

		if (Y[i] < 0) {            //  0을 받았다고 판단 한 경우
			R0++;                  //    0을 받았으므로 R0를 1 증가시킨다.
			if (X[i] > 0)          //    0을 받은 상태인데 1을 보낸 경우라면 
				S1R0++;            //    S1R0를 증가시킨다.
		}
		else if (Y[i] >= 0) {      //  1을 받았다고 판단 한 경우
			R1++;                  //    1을 받았으므로 R1를 1 증가시킨다.    
			if (X[i] <= 0)         //    1을 받은 상태인데 0을 보낸 경우라면 
				S0R1++;            //    S0R1를 증가시킨다.
		}
	}

	double Y_gap = (Y_max - Y_min) / PDF_RANGE; //  Y의 pdf를 정하는 구간의 값
	double N_gap = (N_max - N_min) / PDF_RANGE; //  N의 pdf를 정하는 구간의 값

	for (int i = 0; i < DATA_NUM; i++) {
		int Y_index = (Y[i] - Y_min) / Y_gap; //  각 Y[i]값들이 해당되는 index의 값을 구한다.
		Y_pdf[Y_index] += 1.0;                //  그 index에 해당되는 Y_pdf를 1 증가시킨다.

		int N_index = (N[i] - N_min) / N_gap; //  각 N[i]값들이 해당되는 index의 값을 구한다.
		N_pdf[N_index] += 1.0;                //  그 index에 해당되는 N_pdf를 1 증가시킨다.
	}

	Y_pdf[PDF_RANGE - 1] += 1.0; // 위 for문에서 Y_max에 대한 값의 처리는 하지 않으므로 따로 처리를 해 주었다.
	N_pdf[PDF_RANGE - 1] += 1.0; // 위 for문에서 N_max에 대한 값의 처리는 하지 않으므로 따로 처리를 해 주었다.

	double Sum_of_Percentage = 0; // Y의 pdf값을 모두 더할 변수이다. 최종적으론 1이 되어야 한다.

	for (int i = 0; i < PDF_RANGE; i++) {
		Y_pdf[i] /= DATA_NUM;  //  현재 Y_pdf[i]에는 이 구간에 해당되는 Y의 개수가 들어가있다.
							   //    따라서 총 DATA개수로 나누면 확률이 된다.
		N_pdf[i] /= DATA_NUM;  //  현재 N_pdf[i]에는 이 구간에 해당되는 N의 개수가 들어가있다.
							   //    따라서 총 DATA개수로 나누면 확률이 된다.
	}

	for (int i = 0; i < PDF_RANGE; i++) {
		Y_index[i] = Y_min + i * Y_gap; //  그래프에서 x축의 값을 해당 구간의 최소값으로 한다.
		Sum_of_Percentage += Y_pdf[i];  //  Y의 pdf값을 더하는 과정이다.

		fprintf(Y_pdf_file, "%f\n", Y_pdf[i]);      // Y_pdf[i]의 값을 위에서 open한 Y_pdf_file.txt에 저장한다.
		fprintf(Y_index_file, "%f\n", Y_index[i]);  // Y_index[i]의 값을 위에서 open한 Y_index_file.txt에 저장한다.

		fprintf(N_pdf_file, "%f\n", N_pdf[i]);      // N_pdf[i]의 값을 위에서 open한 N_pdf_file.txt에 저장한다.
	}

	cout << " Y의 pdf의 총 합 : " << Sum_of_Percentage << '\n' << '\n'; // Y의 pdf 값을 모두 더한 값을 출력한다.
	cout << " * 현재 v 의 값 : " << v << '\n';
	cout << " * 현재 p 의 값 : " << p << '\n';

	// ******************* 6번 *******************
	cout << '\n';
	cout << " ********** 6번 **********" << '\n';
	cout << " Noise 접촉 후 0이라 판단 : " << R0 << '\n';
	cout << " Noise 접촉 후 1이라 판단 : " << R1 << '\n';

	// ******************* 7번 *******************
	cout << '\n';
	cout << " ********** 7번 **********" << '\n';
	cout << " 0을 보냈는데 1을 받은 횟수 : " << S0R1 << '\n';
	cout << " 1을 보냈는데 0을 받은 횟수 : " << S1R0 << '\n' << '\n';

	cout << " 0을 보냈는데 1로 받았을 확률 : " << double(S0R1) / double(DATA_NUM) << '\n'; // 이게 m, a 가 몇인지 알아야 함.
	cout << " 1을 보냈는데 0로 받았을 확률 : " << double(S1R0) / double(DATA_NUM) << '\n'; // 이게 m, a 가 몇인지 알아야 함.

	// ******************* 8번 *******************
	cout << '\n';
	cout << " ********** 8번 **********" << '\n';
	cout << " 0을 보냈는데 1으로 받을 확률 : " << Gaussian_Integral(1 - p, v, var, 0, INF) << '\n';
	cout << " 1을 보냈는데 0으로 받을 확률 : " << Gaussian_Integral(p, minus_v, var, M_INF, 0) << '\n';

	/*   cout << '\n';
	cout << " ********** 9번 **********" << '\n';
	cout << " 0을 보냈는데 1으로 받을 확률 : " << Gaussian_Integral(1 - p, v, var, 0, INF) << '\n';
	cout << " 1을 보냈는데 0으로 받을 확률 : " << Gaussian_Integral(p, minus_v, var, M_INF, 0) << '\n';*/

	//cout << '\n';
	//cout << " ********** 10번 **********" << '\n';
	//cout << " 0을 보냈는데 1으로 받을 확률 : " << Gaussian_Integral(1 - p, v, var, 0, INF) << '\n';
	//cout << " 1을 보냈는데 0으로 받을 확률 : " << Gaussian_Integral(p, minus_v, var, M_INF, 0) << '\n';

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