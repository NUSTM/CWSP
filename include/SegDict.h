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

#ifndef SEGDICT_H_
#define SEGDICT_H_

#include "Config.h"
#include "StrFun.h"

namespace cwsp
{
    class SegDict
    {
    public:
        SegDict(const char *DictFileName);
        SegDict();
        ~SegDict();

        pair<int, string> GetDictInfo(const char *feat);       // get dictionary feat info
        int size();
        bool LoadDictFile(const char *DictFileName);
        bool SaveDictFile();
        bool ConvertToBinaryFile(const char* InputFileName, const char* OutputFileName);
    private:
        bool ReadFile(const char* FileName);
        bool ReadBinaryFile(const char* FileName);
    private:
        map<string, pair<int, string> > *_dict;
        typedef map<string, pair<int, string> >::iterator _Str2PairMapIter;
    };
}

#endif