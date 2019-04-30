
#include "Pgm.h"
#include <iostream>
#include <fstream>
#include <regex>

Pgm::Pgm()
{
	init();
}

Pgm::~Pgm()
{
	if (this->data != NULL)
	{
		delete[] this->data;
		this->data = NULL;
	}
}

bool Pgm::load(const std::string &filename)
{
	return this->parseFromFile(filename);
}

bool Pgm::save(const std::string &filename)
{
	std::fstream file;
	// 以写入模式打开
	file.open(filename, std::ios::out);

	if (!file.is_open())
	{	//如果文件不可写，则返回false
		std::cout << "The file '" << filename << "' open failed, may be can not be written." << std::endl;
		return false;
	}

	file << this->toPgmString();
	file.close();

	return true;
}

void Pgm::setVersion(const std::string& version)
{
	this->version = version;
}

std::string Pgm::getVersion()
{
	return this->version;
}

void Pgm::setWidth(const int& width)
{
	this->width = width;
}

int Pgm::getWidth()
{
	return this->width;
}

void Pgm::setHeight(const int& height)
{
	this->height = height;
}

int Pgm::getHeight()
{
	return this->height;
}

void Pgm::setMaxValue(const unsigned char & maxValue)
{
	this->maxValue = maxValue;
}

unsigned char Pgm::getMaxValue()
{
	return this->maxValue;
}

void Pgm::setData(unsigned char * data, const int& length)
{
	// 如果数据中已经存在数据，要重新设置数据，先释放原有数据的内存
	if (this->data != NULL)
	{
		delete[] this->data;
		this->data = NULL;
	}

	this->data = new unsigned char[length];
	memcpy(this->data, data, length);
	this->dataLength = length;

	// 解析并设置最大值
	this->parseMaxValueByData();
}

unsigned char* Pgm::getData()
{
	return this->data;
}

std::string Pgm::toPgmString()
{
	std::string content = this->version;
	content += "\n";
	content += std::to_string(this->width);
	content += " ";
	content += std::to_string(this->height);
	content += "\n";
	content += std::to_string(this->maxValue);
	content += "\n";

	for (int i = 0;i < this->dataLength;i++)
	{
		content += std::to_string((int)this->data[i]);
		content += " ";
	}

	return content;
}

void Pgm::init()
{
	this->version = "P2";
	this->width = 0;
	this->height = 0;
	this->maxValue = 0;
	this->data = NULL;
	this->dataLength = 0;
}

void Pgm::parseMaxValueByData()
{
	if (this->dataLength < 1)
	{
		return;
	}

	unsigned char tmp = this->data[0];
	for (int i = 1;i < this->dataLength;i++)
	{
		if (tmp < this->data[i])
		{
			tmp = this->data[i];
		}
	}
	this->setMaxValue(tmp);
}

bool Pgm::parseFromFile(const std::string & filename)
{
	std::fstream file;
	file.open(filename, std::ios::in);
	if (!file.is_open())
	{	//文件打开失败
		std::cout << "The file '" << filename << "' open failed." << std::endl;
		return false;
	}

	std::string line = "";
	std::string pgmData = "";
	int lineIndex = 0;
	while (std::getline(file, line))
	{
		// 如果该行以#开头，则认为该行属于注释，不应该被加载，不进行解析
		if(line.at(0) == '#')
		{
			continue;
		}

		lineIndex++;

		// 解决换行符为\r\n时，getline残留的\r问题，统一移除掉
		if (line.at(line.length() - 1) == '\r')
		{
			line = line.substr(0, line.length() - 1);
		}

		switch (lineIndex)
		{
		case 1://解析第一行：pgm版本
			if (line.find_first_of('P') == 0)
			{
				this->version = line;
			}
			else
			{
				return false;
			}
			break;
		case 2://解析第二行：pgm图片宽度、高度
		{
            // 移除行末的空格，再做正则表达式匹配
            while (line.at(line.length() - 1) == ' ')
            {
                line = line.substr(0, line.length() - 1);
            }
            // 正则表达式匹配
			std::regex regularExpression("^[0-9]* [0-9]*$");
			std::smatch lineMatch;
			if (std::regex_match(line, lineMatch, regularExpression))
			{
				// 找到第一个空格的位置
				int separatorIndex = line.find_first_of(" ");

				// 解析宽度
				std::string widthString = line.substr(0, separatorIndex);
				this->width = atoi(widthString.c_str());

				// 解析高度
				std::string heightString = line.substr(separatorIndex + 1, line.length() - separatorIndex - 1);
				this->height = atoi(heightString.c_str());
			}
			else
			{
				return false;
			}
		}
		break;
		case 3://解析第三行：pgm图片数据体中的最大值
		{
			std::regex regularExpression("^[0-9]*$");
			std::smatch lineMatch;
			if (std::regex_match(line, lineMatch, regularExpression))
			{
				this->maxValue = atoi(line.c_str());
			}
			else
			{
				return false;
			}
		}
		break;
		default://解析第四行及以后为 “pgm数据体” ，先缓存到pgmData中，再做解析
			if (line.at(line.length() - 1) != ' ')
			{	//如果行末尾无空格，则主动加空格，为接下根据空格解析做准备
				line += ' ';
			}
			pgmData += line;
			break;
		}
	}

	// 根据空格解析数据体
	// 先建立一个vector缓存数据，待解析完毕再转存到this->data中
	std::vector<unsigned char> tmp;
	// 找第一个空格
	int spaceIndex = pgmData.find_first_of(" ");
	while (spaceIndex >= 0)
	{	
		// 如果空格不在第一个元素，空格之前的数据为数据体的有效数据
		if (spaceIndex > 0)
		{
			tmp.push_back(atoi(pgmData.substr(0, spaceIndex).c_str()));
		}
		
		// 移除已缓存过的有效数据
		pgmData = pgmData.substr(spaceIndex + 1, pgmData.length() - spaceIndex - 1);
		// 不断找第一个空格
		spaceIndex = pgmData.find_first_of(" ");

		// 直到找到的第一个空格在数据末尾，则认为已经解析完成，缓存最后一个数据，并跳出while
		if (spaceIndex == pgmData.length() - 1)
		{
			tmp.push_back(atoi(pgmData.substr(0, spaceIndex).c_str()));
			break;
		}
	}
	// 为this->data开辟内存，存数据
	this->dataLength = tmp.size();
	this->data = new unsigned char[this->dataLength];
	for (int i = 0;i<this->dataLength;i++)
	{
		this->data[i] = tmp.at(i);
	}

	return true;
}
