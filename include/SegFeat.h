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

#ifndef SEGFEAT_H_
#define SEGFEAT_H_

#include "Config.h"
#include "StrFun.h"
#include "SegVocab.h"
#include "SegDict.h"
#include "CharType.h"
// #include <algorithm>

namespace cwsp
{
    class SegFeat
    {
    public:
        SegFeat();
        ~SegFeat();
        int GetUnigramIndex(const char *feat);  // get unigram feat's index
        int UnigramLen()
        {
            return _unigram->size();
        }
        void SetModifiable(bool flag) {
            _modifiable = flag;
            std::cout<<"SegFeat cannot append new features now!"<<endl;
        }

        int InsertUnigramFeat(const char *feat);   //Insert and get the index of Unigram

        bool LoadDictFile(const char *DictFileName);
        bool LoadCharFile(bool is_bin);
        bool LoadFeatureFile(const char *UnigramFileName);
        bool SaveFeatureFile();
        bool ConvertToBinaryFile(const char* InputFileName, const char* OutputFileName);

        void GenerateFeats(vector<string> charVec, vector<vector<int> > &featsVec);
        void Feature2vec(vector<vector<string> > feats, vector<vector<string> > &featsVec);

    private:
        bool ReadFile(const char* FileName);
        bool ReadBinaryFile(const char* FileName);
    private:
        Vocab *_unigram;
        SegDict *_dict;
        CharType *_char_type;
        bool _modifiable;    // if _modifiable is "false", you cannot insert new feature
    };
}

#endif