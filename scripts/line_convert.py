#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on 14:27:56 2016-08-09

@author: heshenghuan (heshenghuan@sina.com)
http://github.com/heshenghuan
"""

import codecs
import sys


def word2pair(input_file, output_file):
    input_data = codecs.open(input_file, 'r', 'utf-8')
    output_data = codecs.open(output_file, 'w', 'utf-8')
    sentences = [line.strip() for line in input_data.readlines()
                 if line.strip() != ""]

    # Remove BOM
    if ord(sentences[0][0]) == 0xfeff:
        sentences[0] = sentences[0][1:]

    num = len(sentences)
    output_data.write("#PairPattern Sentence\n%d\n" % num)
    for line in sentences:
        word_list = line.strip().split()
        for word in word_list:
            if len(word) == 1:
                output_data.write(word + " S\n")
            else:
                output_data.write(word[0] + " B\n")
                for w in word[1:len(word) - 1]:
                    output_data.write(w + " M\n")
                output_data.write(word[len(word) - 1] + " E\n")
        output_data.write("\n")
    input_data.close()
    output_data.close()


def split_word(input_file, output_file):
    input_data = codecs.open(input_file, 'r', 'utf-8')
    output_data = codecs.open(output_file, 'w', 'utf-8')
    sentences = [line.strip() for line in input_data.readlines()
                 if line.strip() != ""]

    # Remove BOM
    if ord(sentences[0][0]) == 0xfeff:
        sentences[0] = sentences[0][1:]

    num = len(sentences)
    output_data.write("#PairPattern Sentence\n%d\n" % num)
    for line in sentences:
        for char in line:
            output_data.write(char + "\n")
        output_data.write("\n")
    input_data.close()
    output_data.close()


def usage():
    print "Convertion Python script for lines to (char tag) pairs."
    print "Please make sure the format of source is correct."
    print "This script only has simple conversion function,",
    print "and CANNOT detect wrong input.\n"
    print "Usage: python line_convert.py MODE SOURCE OUTPUT"
    print "       MODE    1: the SOURCE file is a segmented file."
    print "               2: the SOURCE file is a unsegmented file."
    print "       SOURCE  The file you want to convert"
    print "       OUTPUT  The path of output file"

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print "ERROR! Not enough parameters!\n"
        usage()
        sys.exit()
    if sys.argv[1] == "1":
        word2pair(sys.argv[2], sys.argv[3])
    elif sys.argv[1] == "2":
        split_word(sys.argv[2], sys.argv[3])
    else:
        print "ERROR! Wrong parameters!\n"
        usage()
        sys.exit()
