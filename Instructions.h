//
// Created by Dragic on 2018/5/22.
//

#ifndef IDCARDREADER_INSTRUCTIONS_H
#define IDCARDREADER_INSTRUCTIONS_H
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <codecvt>
#include <locale>
#include <string>
#include "Information.h"
using namespace std;
#define COMMAND_LENGTH 10
#define SUCCESS_LENGTH 3

class Instructions {
public:
    //命令的unsigned char数组，每个元素存储1字节指令
    unsigned char find[COMMAND_LENGTH];
    unsigned char select[COMMAND_LENGTH];
    unsigned char readBasic[COMMAND_LENGTH];
    unsigned char readComplex[COMMAND_LENGTH];
    unsigned char find_success[SUCCESS_LENGTH];
    unsigned char select_success[SUCCESS_LENGTH];
    unsigned char read_basic_success[SUCCESS_LENGTH];
    Instructions();
    int findCardSuccessfully(char* value);
    int selectCardSuccessfully(char* value);
    int readBasicSuccessfully(char* value);
    void parseInformation(char* value);

    const Information &getInformation() const;

    void setInformation(const Information &information);

    void printHexStr(char *value, unsigned length);

private:
    Information information;
};


#endif //IDCARDREADER_INSTRUCTIONS_H
