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

#ifndef STR_FUN_H_
#define STR_FUN_H_

#include "Config.h"

namespace cwsp
{
    template <class T> std::string toString(const T& a)
    {
        std::ostringstream ost;
        ost << a;
        return ost.str();
    }

    template<typename T>T fromString( const std::string& s)
    {
        std::istringstream is(s);
        T t;
        is >> t;
        return t;
    }

    vector<string> string_split(string terms_str, string spliting_tag);

    void TrimLine(string & line);
}

#endif


