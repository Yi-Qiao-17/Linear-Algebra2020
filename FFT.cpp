#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;

#define PI 3.1415926
//#define SAMPLENUMBER 8//改用int
//#define LOGNUM 3//int
int SAMPLENUMBER = 0;
int LOGNUM = 0;
int counter = 1;
void InitForFFT();//建立參數

vector<float>Input, Data;
vector<float>fWaveR, fWaveI;
vector<float>sin_tab, cos_tab;
//float Input[SAMPLENUMBER], Data[SAMPLENUMBER];//vector
//float fWaveR[SAMPLENUMBER], fWaveI[SAMPLENUMBER], w[SAMPLENUMBER];//vector
//float sin_tab[SAMPLENUMBER], cos_tab[SAMPLENUMBER];//vector
void FFT(vector<float> dataR, vector<float> dataI);

int main(int argc, char* argv[]) {
	int i, j;
	string row,input_wave;
	stringstream ss,ss1;	
	ifstream fin;
	fin.open(argv[1]/*"ex1.csv"*/);
	while (getline(fin, row)) {//資料各列存到s陣列
		//read////////////////////////////////////////////
		ss.str("");
		ss.clear();
		ss << row;
		while (getline(ss, input_wave, ','))
		{
			float num;
			ss1.str("");
			ss1.clear();
			ss1 << input_wave;
			ss1 >> num;
			Input.push_back(num);
			SAMPLENUMBER++;

		}
		int temp = SAMPLENUMBER;
		while (temp / 2 != 0) {
			temp /= 2;
			LOGNUM++;
		}
		//read/////////////////////////
		InitForFFT();
		for (i = 0; i < SAMPLENUMBER; i++) {
			fWaveR.push_back(Input[i]);
			fWaveI.push_back(0.0f);
			/*fWaveR[i] = Input[i];
			fWaveI[i] = 0.0f;*/
			//w[i] = 0.0f;
		}//輸入訊號放入
		cout << "Data " << counter << " :" << endl;
		FFT(fWaveR, fWaveI);//包含輸出

		

		SAMPLENUMBER = 0;
		LOGNUM = 0;
		Input.clear();
		Data.clear();
		fWaveR.clear();
		fWaveI.clear();
		//w.clear();
		sin_tab.clear();
		cos_tab.clear();

		counter++;
	}
	
	
	return 0;
}
void InitForFFT() {
	int i;
	for (i = 0; i < SAMPLENUMBER; i++) {
		sin_tab.push_back(sin(PI * 2 * i / SAMPLENUMBER));
		cos_tab.push_back(cos(PI * 2 * i / SAMPLENUMBER));
		/*sin_tab[i] = sin(PI * 2 * i / SAMPLENUMBER);
		cos_tab[i] = cos(PI * 2 * i / SAMPLENUMBER);*/
	}//W
}
void FFT(vector<float> dataR, vector<float> dataI) {
	int i, j/*跳超過要回來*/, k, b/*2的(L-1)次方*/, p/*求蝴蝶交叉裡面的係數*/, L/*第幾個stage*/, sum, g;
	/* 0<=j<b */
	vector<int>Bit;//size=lognum
	float TR, TI, temp;
	for (i = 0; i < LOGNUM; i++) {
		Bit.push_back(0);
		//Bit[i] = 0;
	}

	/////位元倒反//////////////////
	for (i = 0; i < SAMPLENUMBER; i++) {
		sum = 0;
		for (g = 0; g < LOGNUM; g++) {
			Bit[g] = (i / (int)pow(2, g)) & 01;
			
		}
		for (g = LOGNUM - 1; g >= 0; g--) {
			sum += (Bit[g] * pow(2, LOGNUM - 1 - g));
		}
		dataI[sum] = dataR[i];
	}
	for (i = 0; i < SAMPLENUMBER; i++) {
		dataR[i] = dataI[i];
		dataI[i] = 0;
	}
	/////位元倒反//////////////////

	//////butterfly////////////////
	for (L = 1; L <= LOGNUM; L++) {
		b = 1;
		i = L - 1;
		while (i > 0) {
			b = b * 2;
			i--;
		}

		for (j = 0; j <= b - 1; j++) {
			p = 1;
			i = LOGNUM - L;
			while (i > 0) {
				p = p * 2;
				i--;
			}
			p = p * j;

			for (k = j; k < SAMPLENUMBER; k = k + 2 * b) {
				TR = dataR[k];
				TI = dataI[k];
				temp = dataR[k + b];
				dataR[k] = dataR[k] + dataR[k + b] * cos_tab[p] + dataI[k + b] * sin_tab[p];
				dataI[k] = dataI[k] - dataR[k + b] * sin_tab[p] + dataI[k + b] * cos_tab[p];
				dataR[k + b] = TR - dataR[k + b] * cos_tab[p] - dataI[k + b] * sin_tab[p];
				dataI[k + b] = TI + temp * sin_tab[p] - dataI[k + b] * cos_tab[p];
			}
		}
	}

	//////butterfly////////////////

	//for (i = 0; i < SAMPLENUMBER; i++) {
	//	cout << sqrt(dataR[i] * dataR[i] + dataI[i] * dataI[i])<<endl;
	//}//答案
	for (i = 0; i < SAMPLENUMBER; i++) {
		cout << fixed << setprecision(4) << (float)dataR[i] << " " << (float)dataI[i]<<endl;
	}//答案

}