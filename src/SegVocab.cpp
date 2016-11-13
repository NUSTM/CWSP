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

#include "SegVocab.h"

namespace cwsp
{
    int Vocab::ParseEngWords(char* sentence, char** words, int max)
    {
        char *word;
        int i = 0;

        for (word = strtok(sentence, g_pcWordSeparators);
            i <= max && word != 0;
            i++, word = strtok(0, g_pcWordSeparators))
        {
            words[i] = word;
        }
        if (i <= max) 
        {
            words[i] = 0;

        }
        else
        {
            words[max]=0;
        }
        return i;

    }

    int Vocab::GetAndInsertWordsIndics(char** words, int* wids, int max)
    {
        int i;

        for (i = 0; i < max && words[i]!=0 && words[i][0] != 0; i++) 
        {
            wids[i] = GetAndInsertIndex(words[i]);
        }
        if( i< max )
        {
            wids[i]= Vocab_None ;
        }
        else
            wids[max-1] = Vocab_None;
        return i;
    }

    int Vocab::GetAndInsertWordsIndics(char** words, vector<int> &wids, int max)
    {
        int i;
        for (i = 0; i < max && words[i]!=0 && words[i][0] != 0; i++) 
        {
            wids.push_back( GetAndInsertIndex(words[i]) );
        }
        wids.push_back( Vocab_None );
        return i;
    }

    int Vocab::GetWordIndics(char** words, int *wids, int max, int &UnkownNum)
    {
        int i;
        UnkownNum = 0;

        for (i = 0; i < max && words[i]!=0 && words[i][0] != 0; i++) 
        {
            wids[i] = GetIndex(words[i]);
            if( _ulUnKnow == wids[i] )
                UnkownNum++;
        }
        if( i< max )
        {
            wids[i]= Vocab_None ;
        }
        else
            wids[max-1] = Vocab_None;
        return i;

    }

    int * Vocab::reverse(int *words)
    {
        int i, j;   /* j can get negative ! */

        for (i = 0, j = length(words) - 1;  i < j;  i++, j--)
        {
            int x = words[i];
            words[i] = words[j];
            words[j] = x;
        }
        return words;
    }

    int Vocab::length(const int *words)
    {
        unsigned int len = 0;

        while (words[len] != Vocab_None) len++;
        return len;
    }

    bool Vocab::WriteVocabIndex(FILE * file)
    {
        map<string, int>::iterator iter;
        for (iter = index_data.begin(); iter != index_data.end(); ++iter)
        {
            string myWord = (*iter).first;
            int unit = (int)myWord.length();
            int myId = (*iter).second;
            fwrite(&unit, sizeof(int), 1, file);
            fwrite(myWord.data(), myWord.length(), 1, file);
            fwrite(&myId, sizeof(int), 1, file);
        }
        return true;
    }

    bool Vocab::WriteVocabText(FILE * file)
    {
        map<string, int>::iterator iter;
        for (iter = index_data.begin(); iter != index_data.end(); ++iter)
        {
            string myWord = (*iter).first + " "+ toString((*iter).second) + '\n';
            fwrite(myWord.data(), myWord.length(), 1, file);
        }
        return true;
    }
}
