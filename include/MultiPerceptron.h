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

#pragma once
#include "Config.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;

namespace cwsp
{
    struct feature
    {
	   vector<int> id_vec;					//index
	   vector<float> value_vec;			//value
    };

    class MultiPerceptron
    {
    public:
    	MultiPerceptron();
        ~MultiPerceptron();
    	bool save_model(string model_file);
        bool save_bin_model(string model_file);
        bool read_model(string model_file);
	    bool load_training_file(string training_file);
        bool load_training_data(vector<feature> feat_vec, vector<int> class_vec);
    	void init_omega();
	
        int train_SGD(size_t max_loop, double loss_thrd, float learn_rate, int avg);
        int train_batch(int max_loop, double loss_thrd, float learn_rate, int avg);
	    vector<float> calc_score(feature &samp_feat);
	    vector<float> score_to_prb(vector<float> &score);
        int score_to_class(vector<float> &score);
        float classify_testing_file(string testing_file, string output_file, int output_format);
        bool classify_samps_withprb(vector<vector<string> >featsVec, vector<vector<double> > &probs);
        bool ConvertToBinaryFile(string InputFileName, string OutputFileName);
    private:
        bool load_bin_model(string model_file);
        bool load_model(string model_file);
        bool read_samp_file(string samp_file, vector<feature> &samp_feat_vec, vector<int> &samp_class_vec);
        bool read_samps_vec(vector<vector<string> >featsVec, vector<feature> &samp_feat_vec);
        void update_omega(int samp_class, feature &samp_feat, float learn_rate);
        void calc_loss(double *loss, float *acc);
        float calc_acc(vector<int> &test_class_vec, vector<int> &pred_class_vec);	
        float sigmoid(float x);
        vector<string> string_split(string terms_str, string spliting_tag);
    private:
       vector<feature> samp_feat_vec;       //sample list
       vector<int> samp_class_vec;          //vec of samples' class
       vector< vector<float> > omega;       //weights matrix
       //vector<string> label_set;          //label set
       int feat_set_size;                   //dimension of feature
       int class_set_size;                  //the num of class
    };
}