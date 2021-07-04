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
    uint16_t bfType = 0x424D;//unsigned char type[2]= {'B', 'M'};//固定的識別代碼，標準規定。
    uint32_t bfSize;//檔案大小 : 包含上述全部資料大小，這也會是直接對BMP右鍵內容看到的容量大小。
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54;//54=14+40//灰階+1024
};//檔案檔頭 關於檔案而不是BMP的內容
struct BmpInfoHeader {
    uint32_t biSize = 40;//檔頭大小
    uint32_t biWidth;//圖片的寬
    uint32_t biHeight;//圖片的高
    uint16_t biPlanes = 1; // 1=defeaul, 0=custom
    uint16_t biBitCount=24;//位元數，彩圖要設RGB 8*3=24，灰階圖設8
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;//總共有幾個像素，如果是彩圖有RGB，除了長x寬還要再 *3
    uint32_t biXPelsPerMeter = 0; // 72dpi=2835, 96dpi=3780
    uint32_t biYPelsPerMeter = 0; // 120dpi=4724, 300dpi=11811
    /*PelsPerMeter指的是密度，一般軟體好像也不看都是設0即可，windwos電腦預設是96dpi，大概就差別在你把這張圖拉上PPT上設越高會縮的越小。*/
    uint32_t biClrUsed = 256;//調色盤的顏色數，彩圖設 0 即可，灰階圖要自訂調色盤要設 256
    uint32_t biClrImportant = 0;
};//圖片檔頭 關於BMP檔的一些資訊如檔案大小、顏色…

 BmpFileHeader bf;
 BmpInfoHeader bi;
 void readmap(int argc, char* argv[]) ;
 void output();
 void process();
 vector <char[24]> temp(72);//Rgb各8個bit=8*3=24，一個字24個bitS
 vector<int[8]>number(72);
 vector<int>toasc(72);//轉成10進制用ascii
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
        content = bf.bfSize - bf.bfOffBits;//內容的btye
    content /= bi.biWidth;//字數
      for (int i = 0; i < content; i++) 
        fin.read(temp[i], bi.biWidth);
  
     fin.close();
}
void output() {
    //顯示檔案資訊
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
    }//將讀檔內容轉成2進位
  
     /*for (i = 0; i < 72; i++) {
        for (j = 0; j < 24; j++)cout << temp[i][j] << " ";
        cout << endl << endl;
    }*/
    for (i = 0; i < 72; i++) {
        int k = 0;
        for (j = 0; j < 24; j += 3) {
            number[i][k++] = temp[i][j] - '0';
        }
    }//將temp的值以三個分為一組，只取每組第一個的值

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
    }//將number中的值轉成10進制
}

