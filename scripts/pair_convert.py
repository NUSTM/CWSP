#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on 14:27:56 2016-08-09

@author: heshenghuan (heshenghuan@sina.com)
http://github.com/heshenghuan
"""

import codecs
import sys


def tag2word(char_vec, tag_vec):
    words = []
    tmp = ''
    for i in range(len(tag_vec)):
        if tag_vec[i] == 'S':
            tmp = char_vec[i]
            words.append(tmp)
            tmp = ''
        elif tag_vec[i] == 'B':
            tmp += char_vec[i]
        elif tag_vec[i] == 'M':
            tmp += char_vec[i]
        else:
            tmp += char_vec[i]
            words.append(tmp)
            tmp = ''
    return words


def convert(src_file, out_file):
    inputs = codecs.open(src_file, mode='r', encoding='utf8')
    outputs = codecs.open(out_file, mode='w', encoding='utf8')
    sentences = inputs.readlines()

    # Remove BOM
    if ord(sentences[0][0]) == 0xfeff:
        sentences[0] = sentences[0][1:]
    length = int(sentences[1])
    sentences = sentences[2:]
    chars = []
    tags = []
    num = 0
    for line in sentences:
        raw = line.strip()
        if raw == '':
            num += 1
            if num % 100 == 0:
                print '.',
                if num % 1000 == 0:
                    print "\t%3.2f%%" % (100.0 * float(num) / float(length))
            words = tag2word(chars, tags)
            for w in words:
                outputs.write(w)
                outputs.write(' ')
            outputs.write('\n')
            chars = []
            tags = []
        else:
            item = raw.split()
            chars.append(item[0])
            tags.append(item[1])
    print "  100.00%"
    print "\n\nConvertion finished! Totally %d sentences." % num


def usage():
    print "Convertion Python script for (char tag) pairs to lines."
    print "Please make sure the format of source is correct."
    print "This script only for Chinese Word Segmentation task's convert,\n",
    print "use 4-tag system to convert, and CANNOT detect wrong input.\n"
    print "Usage: python pair_convert.py SOURCE OUTPUT"
    print "       SOURCE    the file you want to convert"
    print "       OUTPUT    the path of output file"

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print "ERROR! Not enough parameters!\n"
        usage()
        sys.exit(0)
    convert(sys.argv[1], sys.argv[2])
