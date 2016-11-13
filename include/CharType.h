/********************************************************************
* cwsp: Chinese Word Segmentation Toolkit used multi-perceptron.
* Copyright (C) 2016 Shenghuan He
* Contact: heshenghuan328@gmail.com

* Permission to use, copy, modify, and distribute this software for
* any non-commercial purpose is hereby granted without fee, provided
* that the above copyright notice appear in all copies and that both
* that copyright notice. 
* It is provided "as is" without express or implied warranty.
*********************************************************************/

#ifndef CHAR_TYPE_H_
#define CHAR_TYPE_H_

#include "Config.h"
#include "StrFun.h"

namespace cwsp
{
	class CharType
	{
	public:
		CharType();
		~CharType();
		bool Initialize(bool binflag = true);
		void Convert2Binary();
		int GetPuncType(string & character);
		int GetCharType(string & character);
		int GetCNameType(string & character);
		int GetFNameType(string & character);
	private:
		bool LoadExtraCorpus();
		bool LoadBinaryExtraCorpus();
		void BuildCharacterType(int num, string characterTypeFile, map<string, int> * myMap);
		void BuildChineseNameType(string surnameFile, string givennameFile);
	private:
		map<string, int> * puncMap;				// 0:is not a punctuation 1: is a punctuation
		map<string, int> * characterTypeMap;	// 0:ANum 1:CNum1, 2:CNum2, 3:English character, 4:date, 5:others;
		map<string, int> * chineseNameMap;		// 0:frequency surname 1: common surname 2:given name 3:both 0+2 4:both 2+3 5:others
		map<string, int> * foreignNameMap;		// 0:not common foreign name character 1:common foreign name character
		typedef map<string, int>::iterator _Str2IntMapIter;
	};
}

#endif




