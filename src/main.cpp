
#include <iostream>

#include "Pgm.h"

using namespace std;

// 写pgm
void writePgmDemo()
{
	Pgm pgm;
	pgm.setVersion("P2");
	pgm.setWidth(10);
	pgm.setHeight(10);
	unsigned char pgmData[100] = { 255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255, 255,255,255,255,255,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255, 255,255,255,255,255,0,0,0,0,0 };
	pgm.setData(pgmData, sizeof(pgmData));

	if (pgm.save("write.pgm"))
	{
		cout << "保存成功" << endl;;
	}
	else
	{
		cout << "保存失败" << endl;
	}
}

// 读pgm
void readPgmDemo()
{
	Pgm pgm;
	pgm.load("write.pgm");
	cout << pgm.toPgmString() << endl;
}

int main()
{
    cout<<"------------pgm demo------------"<<endl;

	//writePgmDemo();

	readPgmDemo();

	cout << "请按任意键退出！" << endl;
	getchar();

    return 0;
}
