
#ifndef _PGM_H_
#define _PGM_H_

#include <string>

/*
	类名：Pgm
	用途：*.pgm生成、解析
	示例：

	// 写pgm
	void writePgmDemo()
	{
		Pgm pgm;
		pgm.setVersion("P2");
		pgm.setWidth(10);
		pgm.setHeight(10);
		unsigned char pgmData[100] = { 255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255, 255,255,255,255,255,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255, 255,255,255,255,255,0,0,0,0,255 };
		pgm.setData(pgmData, sizeof(pgmData));

		if (pgm.save("write.pgm"))
		{
			std::cout << "保存成功" << std::endl;;
		}
		else
		{
			std::cout << "保存失败" << std::endl;
		}
	}

	// 读pgm
	void readPgmDemo()
	{
		Pgm pgm;
		pgm.load("write.pgm");
		std::cout << pgm.toPgmString() << std::endl;
	}
 */
class Pgm
{
public:
	Pgm();
	~Pgm();

	// 加载图片，filename: 文件名（绝对路径、相对路径）
	bool load(const std::string& filename);

	// 保存图片，filename: 文件名（绝对路径、相对路径）
	bool save(const std::string& filename);

	// 设置pgm版本，默认为P2
	void setVersion(const std::string& version = "P2");

	// 获取pgm版本
	std::string getVersion();

	// 设置pgm图片宽度
	void setWidth(const int& width);

	// 获得pgm图片宽度
	int getWidth();

	// 设置pgm图片高度
	void setHeight(const int& height);

	// 获得pgm图片高度
	int getHeight();

	// 设置pgm图片数据体中的最大值(deprecated)，最大值默认会在setData中解析获得
	void setMaxValue(const unsigned char& maxValue);

	// 获得pgm图片数据体中的最大值
	unsigned char getMaxValue();

	// 设置pgm图片的数据体，data数据首地址，length数据长度
	void setData(unsigned char* data, const int& length);

	// 获得pgm图片的数据体
	unsigned char* getData();

	// 格式化成pgm文件内容（本质为字符串）
	std::string toPgmString();

private:
	// 初始化
	void init();
	// 从pgm图片的数据体中解析出最大值，并设置
	void parseMaxValueByData();
	// 解析pgm文件
	bool parseFromFile(const std::string& filename);

private:
	std::string version;
	int width;
	int height;
	unsigned char maxValue;
	unsigned char* data;
	int dataLength;
};


#endif

