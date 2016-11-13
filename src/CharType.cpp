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

#include "CharType.h"

namespace cwsp
{

    CharType::CharType()
    {
        puncMap = new map<string, int>;
        characterTypeMap = new map<string, int>;
        chineseNameMap = new map<string, int>;
        foreignNameMap = new map<string, int>;
    }

    CharType::~CharType()
    {
        delete puncMap;
        delete characterTypeMap;
        delete chineseNameMap;
        delete foreignNameMap;
    }

    bool CharType::Initialize(bool binflag /* = true */)
    {
        if (binflag)
        {
            return LoadBinaryExtraCorpus();
        }
        else
        {
            return LoadExtraCorpus();
        }
    }

    void CharType::Convert2Binary()
    {
        LoadExtraCorpus();

#ifdef WIN32
        string _datapath = "data\\";
#else
        string _datapath = "data/";
#endif

        string binarySource = _datapath + "binarySource";
        FILE* bin_chartype_file;
        bin_chartype_file=fopen(binarySource.c_str(),"wb");
        int pucSize = (int)puncMap->size();
        fwrite(&pucSize, sizeof(int), 1, bin_chartype_file);
        _Str2IntMapIter iter = puncMap->begin();
        for (; iter != puncMap->end(); ++iter)
        {
            string myPunc = iter->first;
            int unit = (int)myPunc.length();
            fwrite(&unit, sizeof(int), 1, bin_chartype_file);
            fwrite(myPunc.data(), unit, 1, bin_chartype_file);
            int myType = iter->second;
            fwrite(&myType, sizeof(int), 1, bin_chartype_file);
        }

        int charSize = (int)characterTypeMap->size();
        fwrite(&charSize, sizeof(int), 1, bin_chartype_file);
        iter = characterTypeMap->begin();
        for (; iter != characterTypeMap->end(); ++ iter)
        {
            string myChar = iter->first;
            int unit = (int)myChar.length();
            fwrite(&unit, sizeof(int), 1, bin_chartype_file);
            fwrite(myChar.data(), unit, 1, bin_chartype_file);
            int myType = iter->second;
            fwrite(&myType, sizeof(int), 1, bin_chartype_file);
        }
        
        int cNameSize = (int)chineseNameMap->size();
        fwrite(&cNameSize, sizeof(int), 1, bin_chartype_file);
        iter = chineseNameMap->begin(); 
        for (; iter != chineseNameMap->end(); ++ iter)
        {
            string myChar = iter->first;
            int unit = (int)myChar.length();
            fwrite(&unit, sizeof(int), 1, bin_chartype_file);
            fwrite(myChar.data(), unit, 1, bin_chartype_file);
            int myType = iter->second;
            fwrite(&myType, sizeof(int), 1, bin_chartype_file);
        }
        
        int fNameSize = (int)foreignNameMap->size();
        fwrite(&fNameSize, sizeof(int), 1, bin_chartype_file);
        iter = foreignNameMap->begin(); 
        for (; iter != foreignNameMap->end(); ++ iter)
        {
            string myChar = iter->first;
            int unit = (int)myChar.length();
            fwrite(&unit, sizeof(int), 1, bin_chartype_file);
            fwrite(myChar.data(), unit, 1, bin_chartype_file);
            int myType = iter->second;
            fwrite(&myType, sizeof(int), 1, bin_chartype_file);
        }
        fclose(bin_chartype_file);
    }

    bool CharType::LoadBinaryExtraCorpus()
    {
#ifdef WIN32
        string _datapath = "data\\";
#else
        string _datapath = "data/";
#endif
        string binarySource = _datapath + "binarySource";
        FILE * bin_source_file;
        bin_source_file = fopen(binarySource.c_str(), "rb");
        if( !bin_source_file )
        {
            cerr << "\nCharType ERROR" << endl;
            cerr << "Can not open the Model File: "<< binarySource <<endl<<endl;
            return false;
        }

        int unit = 0;
        int keySize = 0;
        string myTextLine;
        int myType = 0;
        fread(&keySize, sizeof(int), 1, bin_source_file);
        for (int i = 0; i < keySize; i ++)
        {
            fread(&unit, sizeof(int), 1, bin_source_file);
            char buf[LINE_LEN_MAX];
            fread((void*)&buf, unit, 1, bin_source_file);
            myTextLine = string(buf, unit);
            fread(&myType, sizeof(int), 1, bin_source_file);
            puncMap->insert(make_pair(myTextLine, myType));
        }

        fread(&keySize, sizeof(int), 1, bin_source_file);
        for (int i = 0; i < keySize; i ++)
        {
            fread(&unit, sizeof(int), 1, bin_source_file);
            char buf[LINE_LEN_MAX];
            fread((void*)&buf, unit, 1, bin_source_file);
            myTextLine = string(buf, unit);
            fread(&myType, sizeof(int), 1, bin_source_file);
            characterTypeMap->insert(make_pair(myTextLine, myType));
        }

        fread(&keySize, sizeof(int), 1, bin_source_file);
        for (int i = 0; i < keySize; i ++)
        {
            fread(&unit, sizeof(int), 1, bin_source_file);
            char buf[LINE_LEN_MAX];
            fread((void*)&buf, unit, 1, bin_source_file);
            myTextLine = string(buf, unit);
            fread(&myType, sizeof(int), 1, bin_source_file);
            chineseNameMap->insert(make_pair(myTextLine, myType));
        }
        
        fread(&keySize, sizeof(int), 1, bin_source_file);
        for (int i = 0; i < keySize; i ++)
        {
            fread(&unit, sizeof(int), 1, bin_source_file);
            char buf[LINE_LEN_MAX];
            fread((void*)&buf, unit, 1, bin_source_file);
            myTextLine = string(buf, unit);
            fread(&myType, sizeof(int), 1, bin_source_file);
            foreignNameMap->insert(make_pair(myTextLine, myType));
        }

        fclose(bin_source_file);
        return true;
    }

    bool CharType::LoadExtraCorpus()
    {

#ifdef WIN32
        string _datapath = "data\\";
#else
        string _datapath = "data/";
#endif

        string puncationfile = _datapath + "Punc";
        BuildCharacterType(1, puncationfile, puncMap);

        string anumfile = _datapath + "ANum";
        BuildCharacterType(0, anumfile, characterTypeMap);

        string cnumfile1 = _datapath + "CNum1";
        BuildCharacterType(1, cnumfile1, characterTypeMap);

        string cnumfile2 = _datapath + "CNum2";
        BuildCharacterType(2, cnumfile2, characterTypeMap);

        string englishcharfile = _datapath + "EChar";
        BuildCharacterType(3, englishcharfile, characterTypeMap);

        string datefile = _datapath + "Date";
        BuildCharacterType(4, datefile, characterTypeMap);

        string surnameFile = _datapath + "CFamilyName";
        string givennameFile = _datapath + "CGivenName";
        BuildChineseNameType(surnameFile, givennameFile);

        string foreignnamefile = _datapath + "ForeignName";
        BuildCharacterType(1, foreignnamefile, foreignNameMap);

        return true;
    }

    void CharType::BuildCharacterType(int num, string characterTypeFile, map<string, int> * myMap)
    {
        ifstream fin;
        fin.open(characterTypeFile.c_str());
        if (!fin.is_open())
        {
            cerr << "\nCharType ERROR" << endl;
            cerr << "Open " << characterTypeFile << " error!" << endl;
            return;
        }
        else
        {
            string myTextLine;
            getline(fin, myTextLine); // Skip the first line

            getline(fin, myTextLine);
            int numSum = fromString<int>(myTextLine);

            for (int i = 0; i < numSum; i ++)
            {
                getline(fin, myTextLine);
                TrimLine(myTextLine);
                _Str2IntMapIter iter = myMap->find(myTextLine);
                if (iter != myMap->end())
                {
                    cerr << characterTypeFile << " is error!" << endl;
                }
                else
                {
                    myMap->insert(make_pair(myTextLine, num));
                }
            }
        }
        fin.close();
    }

    void CharType::BuildChineseNameType(string surnameFile, string givennameFile)
    {
        ifstream finSur, finGiv;
        finSur.open(surnameFile.c_str());
        finGiv.open(givennameFile.c_str());
        if (!finSur.is_open())
        {
            cerr << "\nCharType ERROR" << endl;
            cerr << "Open " << surnameFile << " error!" << endl;
            return;
        }
        else if (!finGiv.is_open())
        {
            cerr << "\nCharType ERROR" << endl;
            cerr << "Open " << givennameFile << " error!" << endl;
            return;
        }
        else
        {
            string myTextLine;
            getline(finSur, myTextLine); // Skip the first line

            getline(finSur, myTextLine);
            int surSum = fromString<int>(myTextLine);
            for (int i = 0; i < surSum; i ++)
            {
                getline(finSur, myTextLine);
                TrimLine(myTextLine);
                _Str2IntMapIter iter = chineseNameMap->find(myTextLine);
                if (iter != chineseNameMap->end())
                {
                    cerr << surnameFile << " is error!" << endl;
                }
                else
                {
                    chineseNameMap->insert(make_pair(myTextLine, 0));
                }
            }

            getline(finSur, myTextLine);
            getline(finSur, myTextLine);
            int surSum2 = fromString<int>(myTextLine);
            for (int i = 0; i < surSum2; i ++)
            {
                getline(finSur, myTextLine);
                TrimLine(myTextLine);
                _Str2IntMapIter iter = chineseNameMap->find(myTextLine);
                if (iter != chineseNameMap->end())
                {
                    cerr << surnameFile << " is error!" << endl;
                }
                else
                {
                    chineseNameMap->insert(make_pair(myTextLine, 1));
                }
            }

            getline(finGiv, myTextLine);
            getline(finGiv, myTextLine);
            int givSum = fromString<int>(myTextLine);
            for (int j = 0; j < givSum; j ++)
            {
                getline(finGiv, myTextLine);
                TrimLine(myTextLine);
                _Str2IntMapIter iter = chineseNameMap->find(myTextLine);
                if (iter != chineseNameMap->end())
                {
                    if (iter->second == 0)
                    {
                        iter->second = 3;
                    }
                    else
                    {
                        iter->second = 4;
                    }
                }
                else
                {
                    chineseNameMap->insert(make_pair(myTextLine, 2));
                }
            }
        }
        finSur.close();
        finGiv.close();
    }

    int CharType::GetPuncType(string & character)
    {
        _Str2IntMapIter iter = puncMap->find(character);
        if (iter != puncMap->end())
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    int CharType::GetCharType(string & character)
    {
        _Str2IntMapIter iter = characterTypeMap->find(character);
        if (iter != characterTypeMap->end())
        {
            return iter->second;
        }
        else
        {
            return 5;
        }
    }

    int CharType::GetCNameType(string & character)
    {
        _Str2IntMapIter iter = chineseNameMap->find(character);
        if (iter != chineseNameMap->end())
        {
            return iter->second;
        }
        else
        {
            return 5;
        }
    }

    int CharType::GetFNameType(string & character)
    {
        _Str2IntMapIter iter = foreignNameMap->find(character);
        if (iter != foreignNameMap->end())
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
