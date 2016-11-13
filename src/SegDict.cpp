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

#include "SegDict.h"

namespace cwsp
{
    SegDict::SegDict(const char *DictFileName)
    {
        _dict = new map<string, pair<int, string> >;
        LoadDictFile(DictFileName);
    }

    SegDict::SegDict()
    {
        _dict =new map<string, pair<int, string> >;
    }

    SegDict::~SegDict()
    {
        delete _dict;
    }

    pair<int, string> SegDict::GetDictInfo(const char *feat)
    {
        _Str2PairMapIter it = _dict->find(feat);

        if( it != _dict->end() )
            return it->second;
        else
            return make_pair(1, "S");
    }

    int SegDict::size()
    {
        return _dict->size();
    }

    bool SegDict::LoadDictFile(const char *DictFileName)
    {
        FILE * DictFile;
        DictFile = fopen(DictFileName, "rb");
        if (!DictFile)
        {
            cerr << "\nSegDict ERROR" << endl;
            cerr << "Can not open the Dictionary file: "<<DictFileName<<endl;
            return false;
        }
        char headBuf[UNIGRAM_LEN_MAX];
        fread(&headBuf, g_Header_Len, 1, DictFile);
        fclose(DictFile);
        string header = string(headBuf, g_Header_Len);
        if(header == g_Model_Header)
        {
            return ReadBinaryFile(DictFileName);
        }
        else
        {
            return ReadFile(DictFileName);
        }
    }

    bool SegDict::SaveDictFile(){
        if (_dict == NULL || _dict->size()==0)
        {
            cerr << "\nSegDict ERROR" << endl;
            cerr << "Does not have any Dictionary information."<<endl;
            return false;
        }
#ifdef WIN32
        string _datapath = "model\\";
#else
        string _datapath = "model/";
#endif
        string FileName = _datapath + "Dict";
        FILE * DictFile = fopen(FileName.c_str(), "w");

        string Header = "#DictFile\n";
        fwrite(Header.data(), Header.length(), 1, DictFile);
        string dictSize = toString(_dict->size()) + "\n";
        fwrite(dictSize.data(), dictSize.length(), 1, DictFile);
        _Str2PairMapIter it;
        for(it = _dict->begin(); it!=_dict->end(); it++)
        {
            string line = it->first + " ";
            line += toString(it->second.first) + " " + it->second.second + "\n";
            fwrite(line.data(), line.length(), 1, DictFile);           
        }
        return true;
    }

    bool SegDict::ReadFile(const char* FileName)
    {
        ifstream fin;
        fin.open(FileName);
        if ( !fin.is_open())
        {
            cerr << "\nSegDict ERROR" << endl;
            cerr << "Can not open the Dictionary file: "<<FileName<<endl;
            return false;
        }

        delete _dict;
        _dict = new map<string, pair<int, string> >;

        string myTextLine;
        getline(fin, myTextLine);
        getline(fin, myTextLine);
        int dictSize = fromString<int>(myTextLine);
        for(int i=0; i<dictSize; i++)
        {
            getline(fin, myTextLine);
            TrimLine(myTextLine);
            istringstream is(myTextLine.c_str());
            string word, mwl, tag;
            is >> word;
            is >> mwl;
            is >> tag;
            pair<int, string> tmp = make_pair(fromString<int>(mwl), tag);
            _dict->insert(make_pair(word, tmp));
        }
        return true;
    }

    bool SegDict::ReadBinaryFile(const char* FileName)
    {
        FILE * DictFile;
        DictFile = fopen(FileName, "rb");
        if (!DictFile)
        {
            cerr << "\nSegDict ERROR" << endl;
            cerr << "Can not open the Dictionary file: "<<FileName<<endl;
            return false;
        }
        char headBuf[UNIGRAM_LEN_MAX];
        fread(&headBuf, g_Header_Len, 1, DictFile);
        string header = string(headBuf, g_Header_Len);

         delete _dict;
        _dict = new map<string, pair<int, string> >;

        size_t dictSize;
        fread(&dictSize, sizeof(size_t), 1, DictFile);
        int unit, t0_len, MWL;
        string word, t0;
        for(int i=0;i<dictSize;i++)
        {
            fread(&unit, sizeof(int), 1, DictFile);
            char buf[UNIGRAM_LEN_MAX];
            fread((void*)&buf, unit, 1, DictFile);
            word = string(buf, unit);
            fread(&MWL, sizeof(int), 1, DictFile);
            fread(&t0_len, sizeof(int), 1, DictFile);
            fread((void*)&buf, t0_len, 1, DictFile);
            t0 = string(buf, t0_len);
            pair<int, string> tmp = make_pair(MWL, t0);
            _dict->insert(make_pair(word, tmp));
        }
        return true;
    }

    bool SegDict::ConvertToBinaryFile(const char* InputFileName, const char* OutputFileName)
    {
        if(!ReadFile(InputFileName)) return false;
        std::cout<< "Load text Dictionary file finished."<<endl;
        FILE* bin_lm_file;
        bin_lm_file = fopen(OutputFileName, "wb");
        fwrite(g_Model_Header.data(), g_Header_Len, 1, bin_lm_file);
        size_t dictSize  = _dict->size();
        fwrite(&dictSize, sizeof(size_t), 1, bin_lm_file);
        _Str2PairMapIter it;
        for(it=_dict->begin();it!=_dict->end();++it)
        {
            string word = it->first;
            int unit = word.length();
            int MWL = it->second.first;
            string t0 = it->second.second;
            int t0_len = t0.length();
            // write the word
            fwrite(&unit, sizeof(int), 1, bin_lm_file);
            fwrite(word.data(), word.length(), 1, bin_lm_file);
            // write the MWL
            fwrite(&MWL, sizeof(int), 1, bin_lm_file);
            // write the t0
            fwrite(&t0_len, sizeof(int), 1, bin_lm_file);
            fwrite(t0.data(), t0.length(), 1, bin_lm_file);
        }
        fclose(bin_lm_file);
        std::cout<< "Convert to binary file finished!"<<endl;
        return true;
    }
}