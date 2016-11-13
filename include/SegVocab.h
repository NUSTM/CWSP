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

#ifndef SEG_VOCAB_H_
#define SEG_VOCAB_H_

#include "Config.h"
#include "StrFun.h"

namespace cwsp
{
	class Vocab
	{
	public:
		Vocab():_VocabSize(0)
		{
			_ulNull = Insert(g_Vocab_NULL);
			_ulUnKnow = Insert(g_Vocab_Unknown);
			_ulSenStart = Insert(g_Vocab_SentStart);
			_ulSenEnd = Insert(g_Vocab_SentEnd);

		}; //初始化；
		
		//查找或加入词号；
		int GetAndInsertIndex(const char *word)                                  
		{

			map<string, int>::iterator it=index_data.find( word );
			if( it != index_data.end() )
				return (*it).second;
			else
				return Insert(word);
		}; 

		void InserWordAndIndex(string word, int index)
		{	
			if ((word == g_Vocab_NULL) || (word == g_Vocab_Unknown) || (word == g_Vocab_SentStart) || (word == g_Vocab_SentEnd)) return;
			vocab_data.insert(map<int, string>::value_type( index, word ) );
			index_data.insert(map<string, int>::value_type( word, index) );
			_VocabSize++;
		}

		int GetIndex( const char *word )
		{
			map<string, int>::iterator it=index_data.find( word );
			if( it != index_data.end() )
				return (*it).second;
			else
				return _ulUnKnow;
		};

		//将词号转化为单词
		string GetWord(int index)
		{
			if( index<_VocabSize )
				return vocab_data[index].c_str();
			else
				return "NULL";
		};

		int size()
		{
			return _VocabSize;
		};

		int GetAndInsertWordsIndics(char** words, int *wids, int max);		//将词转化为词号或插入
		int GetAndInsertWordsIndics(char** words, vector<int> &wids, int max);
		int GetWordIndics(char** words, int *wids, int max, int &UnkownNum);	//将此转化为词号，并统计未知词的数量
		static int ParseEngWords(char* sentence, char* * words, int max);
		static int *reverse(int *words);									//将词号逆转
		static int length(const int *words);								//计算句子的长度


		 int& UnNull(){ return _ulNull; };			//返回UnKnow Word的词号
		 int& UnKnow(){ return _ulUnKnow; };          //返回UnKnow Word的词号
		 int& SenStart(){ return _ulSenStart; };      //返回<s>的词号
		 int& SenEnd(){ return _ulSenEnd; };         //返回</s>的词号

		 bool WriteVocabIndex(FILE * file);
		 bool WriteVocabText(FILE * file);

	private:
		int Insert(string word)         //将单词插入到单词表中，并返回词号
		{
			_VocabSize++;
			vocab_data.insert(map<int, string>::value_type( _VocabSize, word ) );
			index_data.insert(map<string, int>::value_type( word, _VocabSize ) );
			return _VocabSize;
		};

		int _VocabSize;					//单词表的大小
		map<int, string> vocab_data;      //单词表，用于将词号转化为单词；
		map<string,int> index_data;		//索引表，用于将单词转化为词号；

		int _ulNull;              //g_Vocab_NULL = "NULL";的 index
		int _ulUnKnow;              //g_Vocab_Unknown = "<unk>";的 index
		int _ulSenStart;			//g_Vocab_SentStart = "<s>";的 index
		int _ulSenEnd;			//g_Vocab_SentEnd = "</s>";的 index
	};
}

#endif



