#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include"HaffmanTree.h"
using namespace std;
typedef long LongType;

struct CharInfo
{
	unsigned char _ch;
	LongType _count;
	string _code;
	CharInfo(const LongType count = 0 )
		:_count(count)
	{}
	CharInfo(const char ch)
		:_ch(ch)
	{}
	bool operator!=(const CharInfo& c)const
	{
		return _count != c._count;
	}
	CharInfo operator+(const CharInfo& c)const
	{
		return CharInfo(_count + c._count);
	}
	bool operator<(const CharInfo& c)const
	{
		return _count < c._count;
	}
};

class FileComparess
{
	void GenerateHaffmanCode(HaffmanNode<CharInfo>* root, string code)
	{
		if (root == NULL)
			return;
		GenerateHaffmanCode(root->_left, code + '0');
		GenerateHaffmanCode(root->_right, code + '1');
		root->_wight._code = code;
		if (root->_left == NULL&&root->_right == NULL)
		{
			_info[root->_wight._ch]._code = code;
		}

	}
	void WriteConfig(const char* filename)
	{
		string conf(filename);
		conf = conf + "config";
		FILE* fcon = fopen(conf.c_str(), "wb");
		for (int i = 0; i < 256; ++i)
		{
			
			if (_info[i]._count)
			{
				fputc(_info[i]._ch, fcon);
				fputc(',', fcon);
				char count[100];
				_itoa(_info[i]._count, count, 10);
				fputs(count, fcon);
				fputc(',', fcon);
				fputs(_info[i]._code.c_str(), fcon);
				fputc('\n', fcon);
			}
		}
		fclose(fcon);
	}
	void ReadConfig(const char* filename, CharInfo* HNarry)
	{
		string conf(filename);
		conf = conf + "config";
		FILE* fread = fopen(conf.c_str(), "rb");
		char str[100];
		while (fgets(str, 100, fread))
		{
			char* ptr = str;
		    unsigned char index = (unsigned char)*ptr;
			if (index == '\n')
			{
				HNarry[index]._ch = index;
				fgets(str, 100, fread);
				char* ptr = str;
				ptr++;
				LongType count = 0;//字符的权值，出现的次数
				while (*ptr != ',' && *ptr)
				{
					count *= 10;
					count += (*ptr - '0');
					ptr++;
				}
				HNarry[index]._count = count;
				ptr++;
				string code(ptr);
				HNarry[index]._code = code;
			}
			else
			{
				HNarry[index]._ch = index;
				ptr += 2;
				LongType count = 0;//字符的权值，出现的次数
				while (*ptr != ',' && *ptr)
				{
					count *= 10;
					count += (*ptr - '0');
					ptr++;
				}
				HNarry[index]._count = count;
				ptr++;
				string code(ptr);
				HNarry[index]._code = code;
			}
		}
	}
public:
	void Comparess(const char* filename)
	{
		FILE* fread = fopen(filename, "rb");
		if (fread == NULL)
		{
			cout << "打开待压缩文件失败" << endl;
			return;
		}
		for (int i = 0; i < 256; i++)
		{
			_info[i]._ch = i;
		}
		unsigned char ch = fgetc(fread);
		while (!feof(fread))
		{
			//在windows下回车是'\r\n'
			if (ch == '\r')
			{
				ch = fgetc(fread);
				if (ch != '\n')
				{
					fseek(fread, -1, SEEK_CUR);
				}
			}
			_info[ch]._count++;
			ch = fgetc(fread);
		}
		HaffmanTree<CharInfo> h(_info, 256, CharInfo());
		HaffmanNode<CharInfo>* root = h.GetRoot();
		string str;
		GenerateHaffmanCode(root, str);

		//重新打开待压缩文件读
		fseek(fread, 0, SEEK_SET);
		ch = fgetc(fread);
		unsigned char data = 0;   //写入的数据
		int bitcount = 7;  //标记移位信息
		//打开文件写压缩后的编码
		string write(filename);
		write = write + ".comparess";
		FILE* fwrite = fopen(write.c_str(), "wb");
		while (!feof(fread))
		{
			if (ch == '\r')
			{
				ch = fgetc(fread);
				if (ch != '\n')
				{
					fseek(fread, -1, SEEK_CUR);
				}
			}
			const char* cur = _info[ch]._code.c_str();
			while (*cur)
			{
				if (bitcount >= 0)
				{
					data = data | ((*cur - '0') << bitcount);
					bitcount--;
				}
				if (bitcount < 0)
				{
					fputc(data, fwrite);
					bitcount = 7;
					data = 0;
				}
				cur++;
			}
			ch = fgetc(fread);
		}
		fputc(data, fwrite);
		//写配置文件
		WriteConfig(filename);
		fclose(fread);
		fclose(fwrite);
	}
	void UnComparess(const char* filename)
	{
		CharInfo HNarry[256];
		//读配置文件
		ReadConfig(filename, HNarry);
		//构造Haffman树
		HaffmanTree<CharInfo> h(HNarry, 256, CharInfo());
		//遍历树，找叶子结点，写输出文件
		HaffmanNode<CharInfo>* root = h.GetRoot();
		HaffmanNode<CharInfo>* cur = root;
		//打开压缩文件读
		string comf(filename);
		comf = comf + ".comparess";
		FILE* fread = fopen(comf.c_str(), "rb");
		unsigned char ch = fgetc(fread);
		FILE* fwrite = fopen("output", "wb");
		//根节点的_count值就是整棵树字符出现的次数
		int readcount = root->_wight._count;
		while (readcount)
		{
			unsigned int tmp = 1;
			int bit = 7;   //移动的位数
			while (bit>=0)
			{
				if (ch & (tmp << bit))
				{
					cur = cur->_right;
					bit--;
				}
				else
				{
					cur = cur->_left;
					bit--;
				}
				//找到叶子结点
				if (cur->_left == NULL&&cur->_right == NULL)
				{
					fputc(cur->_wight._ch, fwrite);
					cur = root;
					readcount--;
					//最后一个字符的编码在最后两个字节当中的情况
					if (!readcount)
					{
						break;
					}
				}
				
			}
			ch = fgetc(fread);
		}
		fclose(fread);
		fclose(fwrite);
	}
private:
	CharInfo _info[256];
};
