# CWSP-cpp

------

## 简介

> CWSP是一个中文分词工具。它使用了多类感知器作为模型进行训练，并用维特比算法进行解码工作。使用C++编程实现。



## 数据格式

CWSP-cpp目前仅支持UTF-8格式文件。

预处理工作将会产生特征文件和感知器训练所需的训练数据。需要注意的是，预处理程序仅能处理半角空格，如果您的文件中包含全角空格请替换它们为半角空格。

分词器使用多类感知器程序生成的模型文件进行分词工作。

## 安装

使用`make`命令可以很容易的进行安装。但请确保所有源文件都存在。

## 使用方法

### 分词

使用命令 `cwsp_segs`使用分词器进行分词：

```sh
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

## 资源文件

### /data目录下

字符类别文件：

```
.
├── data
│   └── binarySource
```

如上所示，所有上述文件都是CharType模型的资源文件，将用于产生给定字符的特征。每个文件都包含一个特定类别的中文字符。例如“年”、“月”、“日”等属于中文中的“Date”类别。

名为“binarySource”的文件是CharType模型所需资源文件的二进制打包文件，它包含了CharType需要的所有类别的字符信息。

### /model目录下

Dict文件、 Feat文件、Model文件和Prob文件：

```
.
├── model
│   ├── Dict
│   ├── Feat
|   ├── Model
│   └── Prob
```

Dict文件存储了CWSP所需的额外字典信息。它应该在训练开始前就给出。

Feat、Model和Prob文件是CWSP训练期间产生的模型文件。

## 更新日志

- 2016-10-18 **版本 1.1.0**
  - 向StrFun中添加了一个新的函数，用于字符串切分
  - 支持的语料格式已经改变，从原始的行文本变为了字符和标注（char, tag）组成的配对
  - 使用scripts文件夹下的line_convert.py脚本可对语料文件转换成要求的格式
  - 更改了Pretreatment及Segmentor中相应的API
- 2016-09-19 **版本 1.0.4**
  - 修复了一个版本号显示错误的bug。
  - 更改了帮助信息。
- 2016-08-22 **版本 1.0.3**
  - MultiPerceptron现在已经支持模型文件的二进制格式或文本格式读写。
  - convert程序已经被重新编写过了，它现在有了一些新功能，在本页面中的**使用方法**章节中查看更多细节。
- 2016-08-09 **版本 1.0.2**
  - 添加了两个用于语料文件格式转换的脚本。
- 2016-04-19 **版本 1.0.1**
  - 修复了一个bug，该bug导致训练中间数据被保存为文本文件。现在中间训练数据仅存在于内存中。
- 2016-04-07 **版本 1.0.0**
  - 初始版本，CWSP，基于多类感知器算法的中文分词器。