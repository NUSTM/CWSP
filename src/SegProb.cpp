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

#include "SegProb.h"

namespace cwsp
{
    SegProb::SegProb()
    {
        _init_prob = new vector<double>;
        _trans_prob = new vector< vector<double> >;
        tag2index = new map<string, int>;

        for(int i=0; i<4; i++)
        {
            _init_prob->push_back(0.0);
            vector<double> tmp (4, 0.0);
            _trans_prob->push_back(tmp);
        }

        tag2index->insert(make_pair("B", 0));
        tag2index->insert(make_pair("M", 1));
        tag2index->insert(make_pair("E", 2));
        tag2index->insert(make_pair("S", 3));
    }

    SegProb::~SegProb()
    {   
        delete _init_prob;
        delete _trans_prob;
        delete tag2index;
    }

    void SegProb::InitProbCount(int tag)
    {
        if (this->_init_prob == NULL)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Error: the init_prob point to NULL!"<<endl;
            return;
        }
        if (tag < g_tagIdLbound || tag > g_tagIdRbound)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Given tag beyond the range of 4-tag-system!"<<endl;
            return;
        }
        (this->_init_prob->at(tag))++;
        return;
    }

    void SegProb::TransProbCount(int s, int d)
    {
        if (this->_trans_prob == NULL)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Error: the trans_prob point to NULL!"<<endl;
            return;
        }
        if (s < g_tagIdLbound || s > g_tagIdRbound || d < g_tagIdLbound|| d > g_tagIdRbound)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Given tag beyond the range of 4-tag-system!"<<endl;
            return;
        }
        (this->_trans_prob->at(s).at(d))++;
        return;
    }

    void SegProb::InitProbCount(string tag)
    {
        int index = tag2index->at(tag);
        return InitProbCount(index);
    }

    void SegProb::TransProbCount(string s, string d)
    {
        int s_index, d_index;
        s_index = tag2index->at(s);
        d_index = tag2index->at(d);
        return TransProbCount(s_index, d_index);
    }

    bool SegProb::CalcAllProb()
    {
        double init_sum = 0.0;
        vector<double> trans_sum (4, 0.0);

        for (int i=0; i<4; i++)
        {
            init_sum += this->_init_prob->at(i);
            for(int j=0;j<4;j++)
                trans_sum[i] += this->_trans_prob->at(i).at(j);
        }

        for (int i=0;i<4;i++)
        {
            this->_init_prob->at(i) /= init_sum;
            for(int j=0;j<4;j++)
                this->_trans_prob->at(i).at(j) /= trans_sum[i];
        }

        for (int i=0;i<4;i++)
        {
            if (this->_init_prob->at(i) != 0.0)
                this->_init_prob->at(i) = log(this->_init_prob->at(i));
            else 
                this->_init_prob->at(i) = LogP_Zero;
            for(int j=0;j<4;j++)
            {
                if (this->_trans_prob->at(i).at(j) != 0.0)
                    this->_trans_prob->at(i).at(j) = log(this->_trans_prob->at(i).at(j));
                else 
                    this->_trans_prob->at(i).at(j) = LogP_Zero;
            }
        }
        return true;
    }

    double SegProb::GetInitProb(int tag)
    {
        if (this->_init_prob == NULL)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Error: the init_prob point to NULL!"<<endl;
            return 0.0;
        }
        if (tag < g_tagIdLbound || tag > g_tagIdRbound)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Given tag beyond the range of 4-tag-system!"<<endl;
            return 0.0;
        }
        return this->_init_prob->at(tag);
    }

    double SegProb::GetTransProb(int s, int d)
    {
        if (this->_trans_prob == NULL)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Error: the trans_prob point to NULL!"<<endl;
            return 0.0;
        }
        if (s < g_tagIdLbound || s > g_tagIdRbound || d < g_tagIdLbound|| d > g_tagIdRbound)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Given tag beyond the range of 4-tag-system!"<<endl;
            return 0.0;
        }
        return this->_trans_prob->at(s).at(d);
    }

    bool SegProb::LoadProbFile(const char *FileName)
    {
		FILE * ProbFile;
		ProbFile = fopen(FileName, "rb");
		if (!ProbFile)
		{
            cerr << "\nSegProb ERROR" << endl;
			cerr << "Can not open the Probability info file: "<<FileName<<endl;
			return false;
		}
		char headBuf[UNIGRAM_LEN_MAX];
		fread(&headBuf, g_Header_Len, 1, ProbFile);
		fclose(ProbFile);
		string header = string(headBuf, g_Header_Len);
		if (header == g_Model_Header)
		{
			return ReadBinaryFile(FileName);
		}
		else
		{
			return ReadFile(FileName);
		}
    }

    bool SegProb::SaveProbFile()
    {
        if (this->_init_prob == NULL || this->_trans_prob == NULL)
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Does not have any probability information." << endl;
            cerr << "The init_prob & trans_prob point to NULL!" << endl;
            return false;
        }
#ifdef WIN32
        string _datapath = "model\\";
#else
        string _datapath = "model/";
#endif
        string FileName = _datapath + "Prob";
        FILE * ProbFile;
        ProbFile = fopen(FileName.c_str(), "w");

        // write init prob
        string Header = "#InitProb\n"; 
        fwrite(Header.data(), Header.length(), 1, ProbFile);
        for(int i=0;i<4;i++)
        {
            string prob;
            if (_init_prob->at(i) == LogP_Zero)
                prob = "-Inf\n";
            else
                prob = toString(_init_prob->at(i)) + "\n";
            fwrite(prob.data(), prob.length(), 1, ProbFile);
        }

        Header = "#TransProb\n";
        fwrite(Header.data(), Header.length(), 1, ProbFile);
        for(int i=0;i<4;i++)
        {
            string prob = "";
            for(int j=0;j<4;j++)
            {
                if (_trans_prob->at(i).at(j) == LogP_Zero)
                    prob += "-Inf ";
                else
                    prob += toString(_trans_prob->at(i).at(j)) + " ";
            }
            prob += "\n";
            fwrite(prob.data(), prob.length(), 1, ProbFile);
        }
        std::cout<<"SegProb file have been saved.\n";
        return true;
    }

    bool SegProb::ConvertToBinaryFile(const char* InputFileName, const char* OutputFileName)
    {
		if (!ReadFile(InputFileName)) return false;
        std::cout<<"Load text Probability file finished."<<endl;
        FILE* bin_lm_file;
        bin_lm_file=fopen(OutputFileName,"wb");
        fwrite(g_Model_Header.data(), g_Header_Len, 1, bin_lm_file);
        int initProbSzie = (int)this->_init_prob->size();
        fwrite(&initProbSzie, sizeof(int), 1, bin_lm_file);
        for(int i=0; i<4; i++)
		{
			fwrite(&this->_init_prob->at(i), sizeof(double), 1, bin_lm_file);
		}

        int transProbSize = (int)this->_trans_prob->size();
        fwrite(&transProbSize, sizeof(int), 1, bin_lm_file);
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				fwrite(&this->_trans_prob->at(i).at(j), sizeof(double), 1, bin_lm_file);
			}
		}
        fclose(bin_lm_file);
		std::cout<<"Convert to binary file finished!"<<endl;
        return true;
    }

    bool SegProb::ReadFile(const char *FileName)
    {
		ifstream fin;
        fin.open(FileName);
        if( !fin.is_open() )
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Can not open the Probability info file: "<<FileName<<endl;
            return false;
        }

        string myTextLine;
        vector<string> tmp;
        delete this->_init_prob;
		delete this->_trans_prob;
        this->_init_prob = new vector<double>;
        this-> _trans_prob = new vector< vector<double> >;

        getline(fin, myTextLine);  // skip the first line
        for(int i=0; i<4; i++)
        {
            getline(fin, myTextLine);
            TrimLine(myTextLine);
			double prob;
			if (myTextLine != "-Inf")
			{
				prob = fromString<double>(myTextLine);
				this->_init_prob->push_back(prob);
			}
			else
			{
				this->_init_prob->push_back(LogP_Zero);
			}
        }

        getline(fin, myTextLine);
        //int bigramSize = fromString<int>(myTextLine);
        for(int i=0; i<4; i++)
        {
            getline(fin, myTextLine);
            TrimLine(myTextLine);
            istringstream is(myTextLine.c_str());
            string tmp;
            vector<double> probs;
            while(is >> tmp){
                if (tmp != "-Inf" && tmp != "-inf")
                {
                    double prob = fromString<double>(tmp);
                    probs.push_back(prob);
                }
                else
                    probs.push_back(LogP_Zero);
            }
            // tmp = SplitString(myTextLine, " ");
			
			// for(int j=0; j<4; j++)
			// {
			// 	if (tmp[j] != "-Inf")
			// 	{
			// 		double prob = fromString<double>(tmp[j]);
			// 		probs.push_back(prob);
			// 	}
			// 	else
			// 	{
			// 		probs.push_back(LogP_Zero);
			// 	}
			// }
            //int index = fromString<int>(tmp.back());
			this->_trans_prob->push_back(probs);
        }
        fin.close();
        return true;
    }

    bool SegProb::ReadBinaryFile(const char *FileName)
    {
		FILE * probFile;
        probFile = fopen(FileName, "rb");
        if( !probFile )
        {
            cerr << "\nSegProb ERROR" << endl;
            cerr << "Can not open the Probability info file: "<<FileName<<endl;
            return false;
        }
        char headBuf[UNIGRAM_LEN_MAX];
        fread(&headBuf, g_Header_Len, 1, probFile);
        string header = string(headBuf, g_Header_Len);
		delete this->_init_prob;
		delete this->_trans_prob;
        this->_init_prob = new vector<double>;
        this-> _trans_prob = new vector< vector<double> >;

		int initProbSize, transProbSize;
		fread(&initProbSize, sizeof(int), 1, probFile);
		for(int i=0; i<4; i++)
		{
			double prob;
			fread(&prob, sizeof(double), 1,probFile);
			this->_init_prob->push_back(prob);
		}

		fread(&transProbSize, sizeof(int), 1, probFile);
		for(int i=0; i<4; i++)
		{
			vector<double> probs;
			for(int j=0; j<4; j++)
			{
				double prob;
				fread(&prob, sizeof(double), 1,probFile);
				probs.push_back(prob);
			}
			this->_trans_prob->push_back(probs);
		}
		return true;
    }
}