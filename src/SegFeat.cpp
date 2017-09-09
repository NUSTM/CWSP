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

#include "SegFeat.h"

namespace cwsp
{
    SegFeat::SegFeat()
    {
        _unigram = new Vocab;
        _dict = new SegDict;
        _char_type = new CharType;
        _modifiable = true;
    }
    
    SegFeat::~SegFeat()
    {
        delete _unigram;
        delete _dict;
        delete _char_type;
    }
    
    int SegFeat::GetUnigramIndex(const char *feat)
    {
        if(_modifiable){
            return this->_unigram->GetAndInsertIndex(feat);
        }
        else{
            return this->_unigram->GetIndex(feat);
        }
    }

    bool SegFeat::LoadDictFile(const char * DictFileName) {
        return _dict->LoadDictFile(DictFileName);
    }

    bool SegFeat::LoadCharFile(bool is_bin) {
        return _char_type->Initialize(is_bin);
    }

    int SegFeat::InsertUnigramFeat(const char *feat)
    {
        return this->_unigram->GetAndInsertIndex(feat);
    }

    bool SegFeat::LoadFeatureFile(const char *FeatureFileName)
    {
        FILE * FeatureFile;
        FeatureFile = fopen(FeatureFileName, "rb");
        if( !FeatureFile )
        {
            cerr << "\nSegFeat ERROR" << endl;
            cerr << "Can not open the Feature Model File: "<<FeatureFileName<<endl;
            return false;
        }
        char headBuf[UNIGRAM_LEN_MAX];
        fread(&headBuf, g_Header_Len, 1, FeatureFile);
        fclose(FeatureFile);
        string header = string(headBuf, g_Header_Len);
        if (header == g_Model_Header)
        {
            return ReadBinaryFile(FeatureFileName);
        }
        else
        {
            return ReadFile(FeatureFileName);
        }
    }

    bool SegFeat::SaveFeatureFile()
    {
        if( ( _unigram==NULL ) || (_unigram->size()<=0) )
        {
            cerr << "\nSegFeat ERROR" << endl;
            cerr << "Does not have any feature in Unigram."<<endl;
            return false;
        }
#ifdef WIN32
        string _datapath = "model\\";
#else
        string _datapath = "model/";
#endif
        string FileName = _datapath + "Feat";
        FILE * FeatureFile;
        FeatureFile = fopen(FileName.c_str(), "w");
        
        // write unigram feature
        string unigramSize = toString(_unigram->size()) + '\n';
        string unigramHeader = "#UnigramFeature\n"; 
        fwrite(unigramHeader.data(), unigramHeader.length(), 1, FeatureFile);
        fwrite(unigramSize.data(), unigramSize.length(), 1, FeatureFile);
        _unigram->WriteVocabText(FeatureFile);

        fclose(FeatureFile);
        std::cout<<"SegFeat file have been saved.\n";
        return true;
    }

    bool SegFeat::ReadFile(const char* FileName)
    {
        ifstream fin;
        fin.open(FileName);
        if( !fin.is_open() )
        {
            cerr << "\nSegFeat ERROR" << endl;
            cerr << "Can not open the Feature model file: "<<FileName<<endl;
            return false;
        }

        string myTextLine;
        vector<string> tmp;
        delete this->_unigram;
        this->_unigram = new Vocab;

        // UnigramFeature
        getline(fin, myTextLine);  // skip the first line
        getline(fin, myTextLine);
        int unigramSize = fromString<int>(myTextLine);
        for(int i=0; i<unigramSize; i++)
        {
            getline(fin, myTextLine);
            TrimLine(myTextLine);
            tmp = string_split(myTextLine, " ");
            string feat = tmp.front();
            int index = fromString<int>(tmp.back());
            this->_unigram->InserWordAndIndex(feat, index);
        }

        fin.close();
        return true;
    }

    bool SegFeat::ReadBinaryFile(const char* FileName)
    {
        FILE * FeatureFile;
        FeatureFile = fopen(FileName, "rb");
        if( !FeatureFile )
        {
            cerr << "\nSegFeat ERROR" << endl;
            cerr << "Can not open the Feature model file: "<<FileName<<endl;
            return false;
        }
        char headBuf[UNIGRAM_LEN_MAX];
        fread(&headBuf, g_Header_Len, 1, FeatureFile);
        string header = string(headBuf, g_Header_Len);

        delete this->_unigram;
        this->_unigram = new Vocab;

        // int unigramSize, bigramSize, trigramSize, dictSize;
        int unigramSize;
        fread(&unigramSize, sizeof(int), 1, FeatureFile);
        int index, unit;
        string word;
        for (int i=0; i<unigramSize; i++)
        {
            fread(&unit, sizeof(int), 1, FeatureFile);
            char buf[UNIGRAM_LEN_MAX];
            fread((void*)&buf, unit, 1, FeatureFile);
            word = string(buf, unit);
            fread(&index, sizeof(int), 1, FeatureFile);
            this->_unigram->InserWordAndIndex(word, index);
        }

        fclose(FeatureFile);
        return true;
    }

    bool SegFeat::ConvertToBinaryFile(const char* InputFileName, const char* OutputFileName)
    {
        if (!ReadFile(InputFileName)) return false;
        std::cout<<"Load text Feature file finished."<<endl;
        FILE* str_lm_file;
        FILE* bin_lm_file;
        str_lm_file=fopen(InputFileName,"r");
        if( !str_lm_file )
        {
            cerr << "\nSegFeat ERROR" << endl;
            cerr << "Can not open the Language Model File: "<<InputFileName<<endl;
            return false;
        }
        bin_lm_file=fopen(OutputFileName,"wb");
        fwrite(g_Model_Header.data(), g_Header_Len, 1, bin_lm_file);
        int unigramSize = this->_unigram->size();
        fwrite(&unigramSize, sizeof(int), 1, bin_lm_file);
        this->_unigram->WriteVocabIndex(bin_lm_file);

        fclose(str_lm_file);
        fclose(bin_lm_file);
        return true;
    }

    void SegFeat::GenerateFeats(vector<string> charVec, vector<vector<int> > &featsVec){
        featsVec.clear();
        for (size_t i = 2; i<charVec.size()-2;i++) {
            vector<int> feat;
            string feature;

            // Pu(0) 0
            // feature = toString(_char_type->GetPuncType(charVec.at(i)));
            // feat.push_back(feature);
            if (_char_type->GetPuncType(charVec.at(i))){
                feat.push_back(0);
            }

            /* type features */
            //T(-1)T(0)T(1) 1
            int index;
            index = 1 + _char_type->GetCharType(charVec.at(i-1));
            index += 6 * _char_type->GetCharType(charVec.at(i));
            index += 36 * _char_type->GetCharType(charVec.at(i+1));
            feat.push_back(index);

            //N(-1)N(0)N(1) 2
            index = 1 + _char_type->GetCNameType(charVec.at(i-1));
            index += 6 * _char_type->GetCNameType(charVec.at(i));
            index += 36 * _char_type->GetCNameType(charVec.at(i+1));
            feat.push_back(index);

            //F(-1)F(0)F(1) 3
            index = 1 + _char_type->GetFNameType(charVec.at(i-1));
            index += 2 * _char_type->GetFNameType(charVec.at(i));
            index += 4 * _char_type->GetFNameType(charVec.at(i+1));
            feat.push_back(index);

            // C-2 4
            feature = "U00:" + charVec.at(i-2);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C-1 5
            feature = "U01:" + charVec.at(i-1);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C0 6
            feature = "U02:" + charVec.at(i);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C1 7
            feature = "U03:" + charVec.at(i+1);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C2 8
            feature = "U04:" + charVec.at(i+2);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);

            // C-2C-1 9
            feature = "B00:" + charVec.at(i-2) + charVec.at(i-1);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C-1C0 10
            feature = "B01:" + charVec.at(i-1) + charVec.at(i);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C0C1 11
            feature = "B02:" + charVec.at(i) + charVec.at(i+1);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C1C2 12
            feature = "B03:" + charVec.at(i+1) + charVec.at(i+2);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);

            // C-1C1 13
            feature = "T00:" + charVec.at(i-1) + charVec.at(i+1);
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);

            /* dict features */
            //get MWL, t0
            pair<int, string> ans = _dict->GetDictInfo(charVec.at(i).c_str());
            // MWL+t0 14
            feature = "MWL:" + toString(ans.first) + ans.second;
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C-1+t0 15
            feature = "C00:" + charVec.at(i-1) + ans.second;
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C0+t0 16
            feature = "C01:" + charVec.at(i) + ans.second;
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);
            // C1+t0 17
            feature = "C02:" + charVec.at(i+1) + ans.second;
            index = GetUnigramIndex(feature.c_str());
            index += TYPE_FEAT_SIZE + CNAME_FEAT_SIZE + FNAME_FEAT_SIZE;
            feat.push_back(index);

            featsVec.push_back(feat);
        }
    }
}