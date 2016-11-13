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

#ifndef CONFIG_H_
#define CONFIG_H_

#ifdef _WIN32
#pragma warning(disable:4796 4996)
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <map>
#include <set>
#include <utility>
#include <cmath>

#include "time.h"
#include "assert.h"

using namespace std;

namespace cwsp
{
    const string g_copyright = "\ncwsp: Chinese Word Segmentation Toolkit used multi-perceptron.\nCopyright (C) 2016.";
    const string g_version = "\nVersion: 1.1.0, Last update: 2016-10-18\n";

    const string g_Model_Header = "#bin,model";
    const int g_Header_Len = (int)g_Model_Header.length();

    char const* const g_pcWordSeparators = " \t\r\n";
    char const* const g_Vocab_NULL = "NULL";
    char const* const g_Vocab_Unknown = "<unk>";
    char const* const g_Vocab_SentStart = "<s>";
    char const* const g_Vocab_SentEnd = "</s>"; 

    const int g_DefaultSegOrder = 3;
    const int g_DefaultPosOrder = 3;
    const double g_DefaultAlpha = 0.3;
    const double LogP_PseudoZero = -99;
    const double LogP_Zero = -HUGE_VAL;
    const double LogP_Inf = HUGE_VAL;
    const double LogP_One = 0.0;
    const int Vocab_None = (int)-1;
    const int g_UnigramUnkSum = 100;
    const int UNIGRAM_LEN_MAX = 32;
    const int LINE_LEN_MAX = 256;
    const int MAX_NGRAM_ORDER = 9;
    const int WORD_LEN_MAX = 100;
    const int MAX_SEG_STATE_NUM = 20;
    const int MAX_POS_STATE_NUM = 30;

    const int g_tagIdLbound = 0;
    const int g_tagIdRbound = 3;

    const int TYPE_FEAT_SIZE = 216;
    const int CNAME_FEAT_SIZE = 216;
    const int FNAME_FEAT_SIZE = 8;
}

#endif