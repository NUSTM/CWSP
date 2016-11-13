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

#ifndef SEGMENTOR_H_
#define SEGMENTOR_H_

#include "Config.h"
#include "StrFun.h"
#include "CharType.h"
#include "SegFeat.h"
#include "SegProb.h"
#include "SegDict.h"
#include "MultiPerceptron.h"

namespace cwsp
{
    class Segmentor
    {
    public:
        Segmentor();
        ~Segmentor();
        bool Initialize(bool is_char_bin, string dictfile, string &featfile, string &probfile, string &mpfile);
        bool Initialize();
        // void SegSentence(string & inputSen, string & outputSen);
        void SegSentence(vector<string> myCharVec, string & outputSen);
        void SegFile(string inputfile, string outputfile);
        // void SetAlpha(double & myAlpha);
    private:
        // bool CheckMerge(deque<int> & x, deque<int> & y);
        // bool CheckEqual(deque<int> & x, deque<int> & y);
        // void RenewLattice(vector<string> & charSeq);
        // void SplitLine(string &line, vector<string> &charVec);
        void ReadSentence(ifstream &fin, vector<string> &charVec);
        void GenerateFeats(vector<string> charVec, vector<vector<string> > &featsVec);
        void Feature2vec(vector<vector<string> > feats, vector<vector<string> > &featsVec);
        void GetEmitProb(vector<vector<string> > featsVec, vector<vector<double> > &emit_prob);
        void Viterbi(vector<vector<string> > &myFeatsVec, vector<string> &tagVec);
        void Tag2Word(vector<string> charVec, vector<string> tagVec, string &line);
        string GetTag(int index);
    private:
        SegFeat *_features;
        SegProb *_probs;
        SegDict *_dict;
        CharType *_char_type;
        MultiPerceptron *_mp;

        bool is_initial;
    };
}


#endif