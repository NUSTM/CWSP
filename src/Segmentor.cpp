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
#include "Segmentor.h"

NUSTM_CWSP* NUSTM_CWSP::Create()
{
    return new cwsp::Segmentor();
}

namespace cwsp
{
    Segmentor::Segmentor()
    {
        _features = new SegFeat;
        _probs = new SegProb;
        _mp = new MultiPerceptron;
    }

    Segmentor::~Segmentor()
    {
        delete _features;
        delete _probs;
        delete _mp;
    }

    bool Segmentor::Initialize()
    {
#ifdef WIN32
        string _datapath = "model\\";
#else
        string _datapath = "model/";
#endif
        bool is_char_bin = false;
        string dictfile = _datapath + "Dict";
        string featfile = _datapath + "Feat";
        string probfile = _datapath + "Prob";
        string mpfile = _datapath + "Model";
        return Initialize(is_char_bin, dictfile, featfile, probfile, mpfile);
    }

    bool Segmentor::Initialize(bool is_char_bin, string dictfile, string &featfile, string &probfile, string &mpfile)
    {
        if(!_features->LoadCharFile(is_char_bin))
        {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "Initialization failed!";
            cerr << "Can not initialize the CharType."<<endl;
            return false;
        }
        std::cout<<"Loading CharType finished." <<endl;

        if(!_features->LoadDictFile(dictfile.c_str()))
        {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "Initialization failed!";
            cerr << "Can not initialize the SegDict."<<endl;
            return false;
        }
        std::cout<<"Loading SegDict finished." <<endl;

        if(!_features->LoadFeatureFile(featfile.c_str()))
        {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "Initialization failed!";
            cerr << "Can not initialize the SegFeat."<<endl;
            return false;
        }
        _features->SetModifiable(false);
        std::cout<<"Loading SegFeat finished." <<endl;

        if(!_probs->LoadProbFile(probfile.c_str()))
        {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "Initialization failed!";
            cerr << "Can not initialize the SegProb."<<endl;
            return false;
        }
        std::cout<<"Loading SegProb finished." <<endl;

        if(!_mp->read_model(mpfile))
        {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "Initialization failed!";
            cerr << "Can not initialize the MultiPerceptron."<<endl;
            return false;
        }
        is_initial = true;
        return true;
    }

    string Segmentor::GetTag(int index)
    {
        string ans;
        switch(index)
        {
        case 0:
            ans = "B"; break;
        case 1:
            ans = "M"; break;
        case 2:
            ans = "E"; break;
        case 3:
            ans = "S"; break;
        default:
            ans = "S"; break;
        }
        return ans;
    }

    void Segmentor::ReadSentence(ifstream &fin, vector<string> &charVec) {
        if (!fin.is_open()) {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "On " << __FILE__ << ", line:" << __LINE__ << endl;
            return;
        }
        string myTextLine;
        charVec.clear();

        charVec.push_back("B_1");
        charVec.push_back("B_0");

        getline(fin, myTextLine);
        TrimLine(myTextLine);
        while(!fin.eof() && myTextLine.size()) {
            // vector<string> item = cwsp::string_split(myTextLine, " ");
            charVec.push_back(myTextLine);
            getline(fin, myTextLine);
            TrimLine(myTextLine);
        }

        charVec.push_back("E_0");
        charVec.push_back("E_1");
        return;
    }

    void Segmentor::SegFile(const char * inputfile, const char * outputfile)
    {
        if (!is_initial)
        {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "On " << __FILE__ << ", line:" << __LINE__ << endl;
            cerr << "The Segmentor have not been initialized yet." <<endl;
            return;
        }
        clock_t begin, finish;
        begin = clock();

        ifstream fin;
        fin.open(inputfile);
        if (!fin.is_open())
        {
            cerr << "\nSegmentor ERROR" << endl;
            cerr << "On " << __FILE__ << ", line:" << __LINE__ << endl;
            cerr << "Cannot open file\"" << inputfile <<"\"\n";
            return;
        }
        else
        {
            ofstream fout;
            fout.open(outputfile);
            string myTextLine;
            getline(fin, myTextLine); // Skip the first line
            getline(fin, myTextLine); // Read the number of sentences
            int Length = fromString<int>(myTextLine);
            int numIndex = 0;
            vector<string> charVec;
            std::cout << "Processing . . . ." << endl;
            while (numIndex < Length) {
                numIndex++;
                if (numIndex%100 == 0) {
                    std::cout << ".";
                    std::cout.flush();
                    if (numIndex%5000 == 0) {
                        float percent = float(numIndex) * 100.0 / float(Length);
                        std::cout << " " << percent << "\%" << endl;
                    }
                }
                ReadSentence(fin, charVec);
                string outputSen;
                SegSentence(charVec, outputSen);
                // TrimLine(outputSen);
                fout << outputSen << endl;
            }
            fout.close();
            std::cout << endl;
            std::cout << numIndex << " sentences in total." << endl;
        }
        fin.close();

        finish = clock();
        std::cout << "It takes " << (double) (finish - begin) / CLOCKS_PER_SEC << " secs." << endl;
    }

    void Segmentor::SegSentence(vector<string> myCharVec, string & outputSen)
    {
        vector<vector<int> > myFeatsVec;
        _features->GenerateFeats(myCharVec, myFeatsVec);
        vector<string> myTagVec;
        Viterbi(myFeatsVec, myTagVec);
        Tag2Word(myCharVec, myTagVec, outputSen);
        TrimLine(outputSen);
    }

    void Segmentor::Viterbi(vector<vector<int> > &myFeatsVec, vector<string> &tagVec)
    {
        size_t n = myFeatsVec.size();
        vector<vector<double> > toward;
        vector<vector<int> > back;

        for (size_t i=0;i<n;i++)
        {
            vector<double> a (4, -HUGE_VAL);
            toward.push_back(a);
            vector<int> b (4, -1);
            back.push_back(b);
        }

        vector<vector<double> > emit_prob;
        GetEmitProb(myFeatsVec, emit_prob);
        // run viterbi algorithm
        for (size_t i=0;i<4;i++)
        {
            toward.at(0).at(i) = _probs->GetInitProb(i) + emit_prob.at(0).at(i);
            back.at(0).at(i) = -2;
        }
        // toward algorithm to calculate each t's state
        for (size_t t=1;t<n;t++)
        {
            for (size_t d=0;d<4;d++)
            {
                double prob = -HUGE_VAL;
                double prob_max = -HUGE_VAL;
                int tag_max = 3; // the index of tag “S”
                for(size_t s=0;s<4;s++)
                {
                    prob = toward.at(t-1).at(s) + _probs->GetTransProb(s, d) + emit_prob.at(t).at(d);
                    if (prob > prob_max)
                    {
                        prob_max = prob;
                        tag_max = s;
                    }
                }
                toward.at(t).at(d) = prob_max;
                back.at(t).at(d) = tag_max;
            }
        }
        // backward algorithm to get the best tag sequence
        deque<int> taglist;
        size_t t = n - 1;
        double prob;
        double prob_max = -HUGE_VAL;
        int tag_max = 3;
        for(size_t i=0;i<4;i++)
        {
            prob = toward.at(t).at(i);
            if (prob > prob_max)
            {
                prob_max = prob;
                tag_max = i;
            }
        }
        taglist.push_back(tag_max);
        while(t>=1)
        {
            int preTag = back.at(t).at(taglist.front());
            taglist.push_front(preTag);
            --t;
        }
        // int tttt = (taglist.size()==n)?1:0;
        // std::cout<<"Check if the length of taglist equal to charVec:"<<tttt<<endl;
        tagVec.clear();
        for(size_t i=0;i<n;i++)
        {
            tagVec.push_back(GetTag(taglist.at(i)));
        }
        if(tagVec.back()=="B")
            tagVec.back() = "S";
        else if (tagVec.back() == "M")
            tagVec.back() = "E";
        return;
    }

    void Segmentor::GetEmitProb(vector<vector<int> > featsVec, vector<vector<double> > &emit_prob)
    {
        _mp->classify_samps_withprb(featsVec, emit_prob);
    }

    void Segmentor::Tag2Word(vector<string> charVec, vector<string> tagVec, string &line)
    {
        line.clear();
        string word;
        for(size_t i=0;i<tagVec.size();i++)
        {
            if(tagVec.at(i)=="S")
            {
                word = charVec.at(i+2);
                // std::cout<<word<<endl;
                line += " " + word;
                word.clear();
            }
            else if(tagVec.at(i)=="B")
            {
                // std::cout<<"B\n";
                word += charVec.at(i+2);
            }
            else if(tagVec.at(i)=="M")
            {
                // std::cout<<"M\n";
                word += charVec.at(i+2);
            }
            else
            {
                // std::cout<<"E\n";
                word += charVec.at(i+2);
                line += " " + word;
                word.clear();
            }
            // std::cout<<line<<endl;
        }
        return;
    }
}