//
// Created by Dragic on 2018/5/22.
//

#include "Instructions.h"

Instructions::Instructions()
        :find{0xAA,0xAA,0xAA,0x96,0x69,0x00,0x03,0x20,0x01,0x22},
         select{0xAA,0xAA,0xAA,0x96,0x69,0x00,0x03,0x20,0x02,0x21},
         readBasic{0xAA,0xAA,0xAA,0x96,0x69,0x00,0x03,0x30,0x01,0x32},
         readComplex{0xAA,0xAA,0xAA,0x96,0x69,0x00,0x03,0x30,0x10,0x23},
         find_success{0x00,0x00,0x9F},
         select_success{0x00,0x00,0x90},
         read_basic_success{0x00,0x00,0x90}
{

}

int Instructions::findCardSuccessfully(char *value) {
    unsigned char* u_value = reinterpret_cast<unsigned char*>(value);
    if(strlen(value) > 0) {
        //7,8,9索引上是3位状态码
        if(u_value[7] == find_success[0] && u_value[8] == find_success[1] && u_value[9] == find_success[2])
            return 1;
        else {
            printf("找卡失败错误码:%02x%02x%02x\n",u_value[7],u_value[8],u_value[9]);
            return -1;
        }

    }
    else {
        return -1;
    }
}

int Instructions::selectCardSuccessfully(char *value) {
    unsigned char* u_value = reinterpret_cast<unsigned char*>(value);
    if(strlen(value) > 0) {
        //7,8,9索引上是3位状态码
        if(u_value[7] == select_success[0] && u_value[8] == select_success[1] && u_value[9] == select_success[2])
            return 1;
        else {
            printf("选卡失败错误码:%02x%02x%02x\n",u_value[7],u_value[8],u_value[9]);
            return -1;
        }
    }
    else {
        return -1;
    }
}

int Instructions::readBasicSuccessfully(char *value) {
    unsigned char* u_value = reinterpret_cast<unsigned char*>(value);
    if(strlen(value) > 0) {
        //7,8,9索引上是3位状态码
        if(u_value[7] == read_basic_success[0] && u_value[8] == read_basic_success[1] && u_value[9] == read_basic_success[2])
            return 1;
        else {
            printf("读基本信息(不含指纹)失败错误码:%02x%02x%02x\n",u_value[7],u_value[8],u_value[9]);
            return -1;
        }
    }
    else {
        return -1;
    }
}

void Instructions::parseInformation(char *value) {
    //前10位是状态等信息，data从索引为14的位置开始
    unsigned info_start = 14;
    //名字长度为30字节(中文)
    const unsigned name_length = 30;
    //性别长度为2字节(性别代码) 0未知 1男 2女 9未说明
    const unsigned sex_length = 2;
    //民族长度为4字节（民族代码） 01-56
    const unsigned nation_length = 4;
    //出生日期长度为16字节(YYYYMMDD)
    const unsigned birthday_length = 16;
    //地址长度为70字节(中文+数字)
    const unsigned address_length = 70;
    //身份证号长度为36字节(身份证号为18个数字，故2字节表示一个数字)
    const unsigned idNumber_length = 36;
    //签发机关长度为30字节(中文)
    const unsigned signingOrganization_length = 30;
    //有效期起始日期(YYYYMMDD)
    const unsigned validityPeriodStart_length = 16;
    //有效期截止日期(YYYYMMDD)
    const unsigned validityPeriodEnd_length = 16;
    //最新住址70字节(中文)
    const unsigned newestAddress_length = 70;


    char* name = (char *)malloc(sizeof(char) * (name_length));
    for (unsigned i = info_start, j = 0; i < (info_start + name_length); ++i, ++j) {
        name[j] = value[i];
    }
    char16_t *c16Name = reinterpret_cast<char16_t *>(name);
    u16string ucs2Name(c16Name);
    wstring_convert<codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;//ucs-2与utf-8互相转换
    string DecodedName = utf8_ucs2_cvt.to_bytes(ucs2Name);
    //cout<<"姓名: "<<DecodedName<<endl;
    information.setName(DecodedName);


    info_start += name_length;
    char *sex = (char *)malloc(sizeof(char) * (sex_length + 1));
    memset(sex,'\0',sex_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + sex_length); ++i, ++j) {
        sex[j] = value[i];
    }
    char16_t *c16Sex = reinterpret_cast<char16_t *>(sex);
    u16string ucs2Sex(c16Sex);
    string DecodedSex = utf8_ucs2_cvt.to_bytes(ucs2Sex);
    //cout<<"性别: "<<DecodedSex<<endl;
    information.setSex(DecodedSex);

    info_start += sex_length;
    char *nation = (char *)malloc(sizeof(char) * (nation_length + 1));
    memset(nation,'\0',nation_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + nation_length); ++i, ++j) {
        nation[j] = value[i];
    }
    char16_t *c16Nation = reinterpret_cast<char16_t *>(nation);
    u16string ucs2Nation(c16Nation);
    string DecodedNation = utf8_ucs2_cvt.to_bytes(ucs2Nation);
    //cout<<"民族: "<<DecodedNation<<endl;
    information.setNation(DecodedNation);

    info_start += nation_length;
    char* birthday = (char *)malloc(sizeof(char) * (birthday_length + 1));
    memset(birthday,'\0',birthday_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + birthday_length); ++i, ++j) {
        birthday[j] = value[i];
    }
    char16_t *c16Birthday = reinterpret_cast<char16_t *>(birthday);
    u16string ucs2Birthday(c16Birthday);
    string DecodedBirthday = utf8_ucs2_cvt.to_bytes(ucs2Birthday);
    //cout<<"出生日期: "<<DecodedBirthday<<endl;
    information.setBirthday(DecodedBirthday);

    info_start+= birthday_length;
    char* address = (char *)malloc(sizeof(char) * (address_length + 1));
    memset(address,'\0',address_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + address_length); ++i, ++j) {
        address[j] = value[i];
    }
    char16_t *c16Address = reinterpret_cast<char16_t *>(address);
    u16string ucs2Address(c16Address);
    string DecodedAddress = utf8_ucs2_cvt.to_bytes(ucs2Address);
    //cout<<"家庭住址: "<<DecodedAddress<<endl;
    information.setAddress(DecodedAddress);


    info_start += address_length;
    char *idNumber = (char *)malloc(sizeof(char) * (idNumber_length + 1));
    memset(idNumber,'\0',idNumber_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + idNumber_length); ++i, ++j) {
        idNumber[j] = value[i];
    }
    char16_t *c16IdNumber = reinterpret_cast<char16_t *>(idNumber);
    u16string ucs2IdNumber(c16IdNumber);
    string DecodedIdNumber = utf8_ucs2_cvt.to_bytes(ucs2IdNumber);
    //cout<<"身份证号: "<<DecodedIdNumber<<endl;
    information.setIdNumber(DecodedIdNumber);


    info_start += idNumber_length;
    char *signingOrganization = (char *)malloc(sizeof(char) * (signingOrganization_length + 1));
    memset(signingOrganization,'\0',signingOrganization_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + signingOrganization_length); ++i, ++j) {
        signingOrganization[j] = value[i];
    }
    char16_t *c16SigningOrganization = reinterpret_cast<char16_t *>(signingOrganization);
    u16string ucs2SigningOrganization(c16SigningOrganization);
    string DecodedSigningOrganization = utf8_ucs2_cvt.to_bytes(ucs2SigningOrganization);
    //cout<<"签发机关: "<<DecodedSigningOrganization<<endl;
    information.setSigningOrganization(DecodedSigningOrganization);

    info_start += signingOrganization_length;
    char *validityPeriodStart = (char *)malloc(sizeof(char) * (validityPeriodStart_length + 1));
    memset(validityPeriodStart,'\0',validityPeriodStart_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + validityPeriodStart_length); ++i, ++j) {
        validityPeriodStart[j] = value[i];
    }
    char16_t *c16ValidityPeriodStart = reinterpret_cast<char16_t *>(validityPeriodStart);
    u16string ucs2ValidityPeriodStart(c16ValidityPeriodStart);
    string DecodedValidityPeriodStart = utf8_ucs2_cvt.to_bytes(ucs2ValidityPeriodStart);
    //cout<<"有效起始日期: "<<DecodedValidityPeriodStart<<endl;
    information.setValidityPeriodStart(DecodedValidityPeriodStart);

    info_start += validityPeriodStart_length;
    char *validityPeriodEnd = (char *)malloc(sizeof(char) * (validityPeriodEnd_length + 1));
    memset(validityPeriodEnd,'\0',validityPeriodEnd_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + validityPeriodEnd_length); ++i, ++j) {
        validityPeriodEnd[j] = value[i];
    }
    char16_t *c16ValidityPeriodEnd = reinterpret_cast<char16_t *>(validityPeriodEnd);
    u16string ucs2ValidityPeriodEnd(c16ValidityPeriodEnd);
    string DecodedValidityPeriodEnd = utf8_ucs2_cvt.to_bytes(ucs2ValidityPeriodEnd);
    //cout<<"有效结束日期: "<<DecodedValidityPeriodEnd<<endl;
    information.setValidityPeriodEnd(DecodedValidityPeriodEnd);

    info_start += validityPeriodEnd_length;
    char *newestAddress = (char *)malloc(sizeof(char) * (newestAddress_length + 1));
    memset(newestAddress,'\0',newestAddress_length + 1);
    for (unsigned i = info_start, j = 0; i < (info_start + newestAddress_length); ++i, ++j) {
        newestAddress[j] = value[i];
    }
    if(newestAddress[0] == 0x20 && newestAddress[1] == 0x00) {
        information.setNewestAddress("无");
    }
    else {
        char16_t *c16NewestAddress = reinterpret_cast<char16_t *>(newestAddress);
        u16string ucs2NewestAddress(c16NewestAddress);
        string DecodedNewestAddress = utf8_ucs2_cvt.to_bytes(ucs2NewestAddress);
        cout<<"最新住址: "<<DecodedNewestAddress<<endl;
        information.setNewestAddress(DecodedNewestAddress);
    }
}

void Instructions::printHexStr(char *value, unsigned length) {
    for (int i = 0; i < length; ++i) {
        printf("(%d - %02x) ",i,value[i]);
    }
    printf("\n");
}

const Information &Instructions::getInformation() const {
    return information;
}

void Instructions::setInformation(const Information &information) {
    Instructions::information = information;
}
