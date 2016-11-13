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

#ifndef SEG_PROB_H
#define SEG_PROB_H

#include "Config.h"
#include "StrFun.h"
#include <cmath>

namespace cwsp
{
    class SegProb
    {
    public:
        SegProb();
        ~SegProb();
        void InitProbCount(int tag);
        void TransProbCount(int s, int d);
        void InitProbCount(string tag);
        void TransProbCount(string s, string d);
        bool CalcAllProb();

        double GetInitProb(int tag);
        double GetTransProb(int s, int d);

        bool LoadProbFile(const char *FileName);
        bool SaveProbFile();
        bool ConvertToBinaryFile(const char* InputFileName, const char* OutputFileName);
    private:
        bool ReadFile(const char *FileName);
        bool ReadBinaryFile(const char *FileName);
    private:
        vector<double> *_init_prob;
        vector< vector<double> > *_trans_prob;

        map<string, int> *tag2index;
    };
}

#endif