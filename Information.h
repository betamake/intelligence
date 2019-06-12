//
// Created by Dragic on 2018/5/30.
//

#ifndef IDCARDREADER_INFORMATION_H
#define IDCARDREADER_INFORMATION_H

#include <string>
#include <ostream>
#include <QString>

using namespace std;

class Information {
public:
    Information();

    Information(const string &name, const string &sex, const string &nation, const string &birthday,
                const string &address, const string &idNumber, const string &signingOrganization,
                const string &validityPeriodStart, const string &validityPeriodEnd, const string &newestAddress);

    const string &getName() const;

    void setName(const string &name);

    const string &getSex() const;

    void setSex(const string &sex);

    const string &getNation() const;

    void setNation(const string &nation);

    const string &getBirthday() const;

    void setBirthday(const string &birthday);

    const string &getAddress() const;

    void setAddress(const string &address);

    const string &getIdNumber() const;

    void setIdNumber(const string &idNumber);

    const string &getSigningOrganization() const;

    void setSigningOrganization(const string &signingOrganization);

    const string &getValidityPeriodStart() const;

    void setValidityPeriodStart(const string &validityPeriodStart);

    const string &getValidityPeriodEnd() const;

    void setValidityPeriodEnd(const string &validityPeriodEnd);

    const string &getNewestAddress() const;

    void setNewestAddress(const string &newestAddress);

    QString toString();

    friend ostream &operator<<(ostream &os, const Information &information);

private:
    string name;
    //性别  0未知 1男 2女 9未说明
    string sex;
    //民族 01-56
    string nation;
    //出生日期(YYYYMMDD)
    string birthday;
    //地址(中文+数字)
    string address;
    //身份证号为18个数字
    string idNumber;
    //签发机关(中文)
    string signingOrganization;
    //有效期起始日期(YYYYMMDD)
    string validityPeriodStart;
    //有效期截止日期(YYYYMMDD)
    string validityPeriodEnd;
    //最新住址70字节(中文)
    string newestAddress;
};


#endif //IDCARDREADER_INFORMATION_H
