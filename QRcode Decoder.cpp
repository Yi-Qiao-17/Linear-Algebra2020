#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
using namespace std;
/*
uint8_t=typedef unsigned char;
uint16_t=typedef unsigned short int;
uint32_t=typedef unsigned int;
uint64_t=typedef unsigned long int;
*/


struct BmpFileHeader {
    uint16_t bfType = 0x424D;//unsigned char type[2]= {'B', 'M'};//�T�w���ѧO�N�X�A�зǳW�w�C
    uint32_t bfSize;//�ɮפj�p : �]�t�W�z������Ƥj�p�A�o�]�|�O������BMP�k�䤺�e�ݨ쪺�e�q�j�p�C
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54;//54=14+40//�Ƕ�+1024
};//�ɮ����Y �����ɮצӤ��OBMP�����e
struct BmpInfoHeader {
    uint32_t biSize = 40;//���Y�j�p
    uint32_t biWidth;//�Ϥ����e
    uint32_t biHeight;//�Ϥ�����
    uint16_t biPlanes = 1; // 1=defeaul, 0=custom
    uint16_t biBitCount=24;//�줸�ơA�m�ϭn�]RGB 8*3=24�A�Ƕ��ϳ]8
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;//�`�@���X�ӹ����A�p�G�O�m�Ϧ�RGB�A���F��x�e�٭n�A *3
    uint32_t biXPelsPerMeter = 0; // 72dpi=2835, 96dpi=3780
    uint32_t biYPelsPerMeter = 0; // 120dpi=4724, 300dpi=11811
    /*PelsPerMeter�����O�K�סA�@��n��n���]���ݳ��O�]0�Y�i�Awindwos�q���w�]�O96dpi�A�j���N�t�O�b�A��o�i�ϩԤWPPT�W�]�V���|�Y���V�p�C*/
    uint32_t biClrUsed = 256;//�զ�L���C��ơA�m�ϳ] 0 �Y�i�A�Ƕ��ϭn�ۭq�զ�L�n�] 256
    uint32_t biClrImportant = 0;
};//�Ϥ����Y ����BMP�ɪ��@�Ǹ�T�p�ɮפj�p�B�C��K

 BmpFileHeader bf;
 BmpInfoHeader bi;
 void readmap(int argc, char* argv[]) ;
 void output();
 void process();
 vector <char[24]> temp(72);//Rgb�U8��bit=8*3=24�A�@�Ӧr24��bitS
 vector<int[8]>number(72);
 vector<int>toasc(72);//�ন10�i���ascii
 uint32_t   content;//(1782-54)/24=72

int main(int argc, char* argv[])
{
    readmap(argc, argv);
    //output();
    process();
    uint32_t  i, j;
    for (i = 0; i < 72; i++) {
        if (toasc[i] >= 32 && toasc[i] < 126)
            cout << (char)toasc[i];
    }
    cout << endl;
    return 0;
}

void readmap(int argc, char* argv[]) {
    ifstream fin;
    fin.open(argv[1]/*"test1.BMP"*/, ios::in | ios::binary);
    if (!fin) {
        cout << "file open error.";
        exit(0);
    }

    /*  fin.open(argv[1], ios::in | ios::binary);*/
     /* fin.read((char*)&bf, sizeof(BmpFileHeader));
      fin.read((char*)&bi, sizeof(BmpInfoHeader));*/
    fin.read((char*)&bf.bfType, sizeof(uint16_t));
    fin.read((char*)&bf.bfSize, sizeof(uint32_t));
    fin.read((char*)&bf.bfReserved1, sizeof(uint16_t));
    fin.read((char*)&bf.bfReserved2, sizeof(uint16_t));
    fin.read((char*)&bf.bfOffBits, sizeof(uint32_t));
    fin.read((char*)&bi.biSize, sizeof(uint32_t));
    fin.read((char*)&bi.biWidth, sizeof(uint32_t));
    fin.read((char*)&bi.biHeight, sizeof(uint32_t));
    fin.read((char*)&bi.biPlanes, sizeof(uint16_t));
    fin.read((char*)&bi.biBitCount, sizeof(uint16_t));
    fin.read((char*)&bi.biCompression, sizeof(uint32_t));
    fin.read((char*)&bi.biSizeImage, sizeof(uint32_t));
    fin.read((char*)&bi.biXPelsPerMeter, sizeof(uint32_t));
    fin.read((char*)&bi.biYPelsPerMeter, sizeof(uint32_t));
    fin.read((char*)&bi.biClrUsed, sizeof(uint32_t));
    fin.read((char*)&bi.biClrImportant, sizeof(uint32_t));
        content = bf.bfSize - bf.bfOffBits;//���e��btye
    content /= bi.biWidth;//�r��
      for (int i = 0; i < content; i++) 
        fin.read(temp[i], bi.biWidth);
  
     fin.close();
}
void output() {
    //����ɮ׸�T
    cout << "FILE_HEADER\n"
        << "bfType=\t\t" << bf.bfType << '\n'
        << "bfSize=\t\t" << bf.bfSize << '\n'
        << "bfReserved1=\t" << bf.bfReserved1 << '\n'
        << "bfReserved2=\t" << bf.bfReserved2 << '\n'
        << "bfOffBits=\t" << bf.bfOffBits << "\n\n"
        << "INFO_HEADER\n"
        << "biSize=\t\t" << bi.biSize << '\n'
        << "biWidth=\t" << bi.biWidth << '\n'
        << "biHeight=\t" << bi.biHeight << '\n'
        << "biPlanes=\t" << bi.biPlanes << '\n'
        << "biBitCount=\t" << bi.biBitCount << '\n'
        << "biCompression=\t" << bi.biCompression << '\n'
        << "biSizeImage=\t" << bi.biSizeImage << '\n'
        << "biXPelsPerMeter=" << bi.biXPelsPerMeter << '\n'
        << "biYPelsPerMeter=" << bi.biYPelsPerMeter << '\n'
        << "biClrUsed=\t" << bi.biClrUsed << '\n'
        << "biClrImportant=\t" << bi.biClrImportant << "\n\n";
   
}
void process() {
    uint32_t  i, j;
    for (i = 0; i < 72; i++) {
        for (j = 0; j < 24; j++) {
            (temp[i][j] == -1) ? temp[i][j] = '0' : temp[i][j] = '1';
        }
    }//�NŪ�ɤ��e�ন2�i��
  
     /*for (i = 0; i < 72; i++) {
        for (j = 0; j < 24; j++)cout << temp[i][j] << " ";
        cout << endl << endl;
    }*/
    for (i = 0; i < 72; i++) {
        int k = 0;
        for (j = 0; j < 24; j += 3) {
            number[i][k++] = temp[i][j] - '0';
        }
    }//�Ntemp���ȥH�T�Ӥ����@�աA�u���C�ղĤ@�Ӫ���

  /*  for (i = 0; i < 72; i++) {
        for (j = 0; j < 8; j++)cout << number[i][j] << " ";
        cout << endl;
    }*/

    for (i = 0; i < 72; i++) {
        int value = 0, z = 7;
        for (j = 0; j < 8; j++) {
            value += (int)number[i][j] * pow(2, z);
            z--;
        }
        toasc[i] = value;
    }//�Nnumber�������ন10�i��
}

