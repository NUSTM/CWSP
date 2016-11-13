# CWSP-cpp

----

## Introduction

> CWSP is a Chinese word segmentation toolkit. It used MultiPerceptron algorithm to train model and the Viterbi algorithm to do decode work. Implemented by c++.

[README in Chinese](./README_zh.md)

## Data format

CWSP-cpp, now, only support UTF-8 file.

Pretreatment will generates feature list and training data for multiPerceptron. **AND Pretreatment program only can handle with half-width space.**

Segmenter use the model file which generate by MultiPerceptron to do segmenetation work.

## Installation

It is easy to type `make`. And make sure all the source code files exist.

## Usage

### Segmentation

Use command `cwsp_seg` to do segmentation work:

```shell
NAME
    cwsp_seg -- Using existing resource files and given model files to
                do segmentation work.
USAGE:
    cwsp_seg [-b 0|1] [-dfp file] model corpus output
OPTIONS:
     -h    Print help infomation

     Following command can be used to specify resources files.
     -b [0,1]    0: CharType resource file is a text file
                 1: CharType resource file is a binary file (default)

     -d <dict>   Specify the dictionary file (default: model\Dict)

     -f <feat>   Specify the feature file (default: model\Feat)

     -p <prob>   Specify the probability file (default: model\Prob)
```

## Resource file

### Under /data

CharType resource files:

	.
	├── data
	│   └── binarySource

All these files are the resources for CharType module, which used to generate features for given character. Each file includes a certain kind of Chinese characters. For example, "年", "月" and "日" are belong to the "Date" type in Chinese.

And the file "binarySource" is a binary resource file for CharType, it includes all types of character CharType needs.

### Under /model

Dict, Feat, Model and Prob:

	.
	├── model
	│   ├── Dict
	│   ├── Feat
  |   ├── Model
	│   └── Prob

Dict stores dictionary informations for CWSP. And it should be given before start training a segmenter.

Feat, Model & Prob are model files generated by CWSP during training processing.


## Change History

- 2016-10-18 **version 1.1.0**
  - Added an function in StrFun, split a string to a vector of strings.
  - Support corpora file format has changed from original line texts to (char, tag) pairs.
  - Script under path "scripts/line_convert.py" can used to do the conversion work.
  - Corresponding API in Pretreatment and Segmentor have been modified.
- 2016-09-19 **version 1.0.4**
  - Fix a wrong version number print bug.
  - Modified the help information.
- 2016-08-22 **version 1.0.3**
  - MultiPerceptron now support both binary and text model file save & load.
  - Convert program had been re-writed, it now has some new feature. See more detail on section **Usage** of this page.
- 2016-08-09 **version 1.0.2**
  - Added 2 scripts of format conversion for corpus file.
- 2016-04-19 **version 1.0.1**
  - Fix a problem, which lead to the training data saved as text file. The training data is only used in memory. 
- 2016-04-07 **version 1.0.0**
  - The first version, CWSP the Chinese Word Segmenter based on Multi-class Perceptron algorithm. 