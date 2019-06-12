//
// Created by Dragic on 2018/5/30.
//

#include "Information.h"

Information::Information() {}

Information::Information(const string &name, const string &sex, const string &nation, const string &birthday,
                         const string &address, const string &idNumber, const string &signingOrganization,
                         const string &validityPeriodStart, const string &validityPeriodEnd,
                         const string &newestAddress) : name(name), sex(sex), nation(nation), birthday(birthday),
                                                        address(address), idNumber(idNumber),
                                                        signingOrganization(signingOrganization),
                                                        validityPeriodStart(validityPeriodStart),
                                                        validityPeriodEnd(validityPeriodEnd),
                                                        newestAddress(newestAddress) {}

const string &Information::getName() const {
    return name;
}

void Information::setName(const string &name) {
    Information::name = name;
}

const string &Information::getSex() const {
    return sex;
}

void Information::setSex(const string &sex) {
//    Information::sex = sex;
    //把性别由数字解析成中文
    if(sex.compare("1") == 0) {
        Information::sex = "男";
    } else if(sex.compare("2") == 0) {
        Information::sex = "女";
    } else if(sex.compare("0") == 0) {
        Information::sex = "未知";
    } else if(sex.compare("9") == 0) {
        Information::sex = "未说明";
    } else {
        Information::sex = "无";
    }
}

const string &Information::getNation() const {
    return nation;
}

void Information::setNation(const string &nation) {
//    Information::nation = nation;
    if(nation.compare("01") == 0) {
        Information::nation = "汉";
    } else if(nation.compare("02") == 0) {
        Information::nation = "蒙古";
    } else if(nation.compare("03") == 0) {
        Information::nation = "回";
    } else if(nation.compare("04") == 0) {
        Information::nation = "藏";
    } else if(nation.compare("05") == 0) {
        Information::nation = "维吾尔";
    } else if(nation.compare("06") == 0) {
        Information::nation = "苗";
    } else if(nation.compare("07") == 0) {
        Information::nation = "彝";
    } else if(nation.compare("08") == 0) {
        Information::nation = "壮";
    } else if(nation.compare("09") == 0) {
        Information::nation = "布依";
    } else if(nation.compare("10") == 0) {
        Information::nation = "朝鲜";
    } else if(nation.compare("11") == 0) {
        Information::nation = "满";
    } else if(nation.compare("12") == 0) {
        Information::nation = "侗";
    } else if(nation.compare("13") == 0) {
        Information::nation = "瑶";
    } else if(nation.compare("14") == 0) {
        Information::nation = "白";
    } else if(nation.compare("15") == 0) {
        Information::nation = "土家";
    } else if(nation.compare("16") == 0) {
        Information::nation = "哈尼";
    } else if(nation.compare("17") == 0) {
        Information::nation = "哈萨克";
    } else if(nation.compare("18") == 0) {
        Information::nation = "傣";
    } else if(nation.compare("19") == 0) {
        Information::nation = "黎";
    } else if(nation.compare("20") == 0) {
        Information::nation = "傈僳";
    } else if(nation.compare("21") == 0) {
        Information::nation = "佤";
    } else if(nation.compare("22") == 0) {
        Information::nation = "畲";
    } else if(nation.compare("23") == 0) {
        Information::nation = "高山";
    } else if(nation.compare("24") == 0) {
        Information::nation = "拉祜";
    } else if(nation.compare("25") == 0) {
        Information::nation = "水";
    } else if(nation.compare("26") == 0) {
        Information::nation = "东乡";
    } else if(nation.compare("27") == 0) {
        Information::nation = "纳西";
    } else if(nation.compare("28") == 0) {
        Information::nation = "景颇";
    } else if(nation.compare("29") == 0) {
        Information::nation = "柯尔克孜";
    } else if(nation.compare("30") == 0) {
        Information::nation = "土";
    } else if(nation.compare("31") == 0) {
        Information::nation = "达斡尔";
    } else if(nation.compare("32") == 0) {
        Information::nation = "仫佬";
    } else if(nation.compare("33") == 0) {
        Information::nation = "羌";
    } else if(nation.compare("34") == 0) {
        Information::nation = "布朗";
    } else if(nation.compare("35") == 0) {
        Information::nation = "撒拉";
    } else if(nation.compare("36") == 0) {
        Information::nation = "毛南";
    } else if(nation.compare("37") == 0) {
        Information::nation = "仡佬";
    } else if(nation.compare("38") == 0) {
        Information::nation = "锡伯";
    } else if(nation.compare("39") == 0) {
        Information::nation = "阿昌";
    } else if(nation.compare("40") == 0) {
        Information::nation = "普米";
    } else if(nation.compare("41") == 0) {
        Information::nation = "塔吉克";
    } else if(nation.compare("42") == 0) {
        Information::nation = "怒";
    } else if(nation.compare("43") == 0) {
        Information::nation = "乌孜别克";
    } else if(nation.compare("44") == 0) {
        Information::nation = "俄罗斯";
    } else if(nation.compare("45") == 0) {
        Information::nation = "鄂温克";
    } else if(nation.compare("46") == 0) {
        Information::nation = "德昂";
    } else if(nation.compare("47") == 0) {
        Information::nation = "保安";
    } else if(nation.compare("48") == 0) {
        Information::nation = "裕固";
    } else if(nation.compare("49") == 0) {
        Information::nation = "京";
    } else if(nation.compare("50") == 0) {
        Information::nation = "塔塔尔";
    } else if(nation.compare("51") == 0) {
        Information::nation = "独龙";
    } else if(nation.compare("52") == 0) {
        Information::nation = "鄂伦春";
    } else if(nation.compare("53") == 0) {
        Information::nation = "赫哲";
    } else if(nation.compare("54") == 0) {
        Information::nation = "门巴";
    } else if(nation.compare("55") == 0) {
        Information::nation = "珞巴";
    } else if(nation.compare("56") == 0) {
        Information::nation = "基诺";
    } else {
        Information::nation = "无";
    }

}

const string &Information::getBirthday() const {
    return birthday;
}

void Information::setBirthday(const string &birthday) {
    Information::birthday = birthday;
}

const string &Information::getAddress() const {
    return address;
}

void Information::setAddress(const string &address) {
    Information::address = address;
}

const string &Information::getIdNumber() const {
    return idNumber;
}

void Information::setIdNumber(const string &idNumber) {
    Information::idNumber = idNumber;
}

const string &Information::getSigningOrganization() const {
    return signingOrganization;
}

void Information::setSigningOrganization(const string &signingOrganization) {
    Information::signingOrganization = signingOrganization;
}

const string &Information::getValidityPeriodStart() const {
    return validityPeriodStart;
}

void Information::setValidityPeriodStart(const string &validityPeriodStart) {
    Information::validityPeriodStart = validityPeriodStart;
}

const string &Information::getValidityPeriodEnd() const {
    return validityPeriodEnd;
}

void Information::setValidityPeriodEnd(const string &validityPeriodEnd) {
    Information::validityPeriodEnd = validityPeriodEnd;
}

const string &Information::getNewestAddress() const {
    return newestAddress;
}

void Information::setNewestAddress(const string &newestAddress) {
    Information::newestAddress = newestAddress;
}

ostream &operator<<(ostream &os, const Information &information) {
    os << "姓名: " << information.name << " \n性别: " << information.sex << " \n民族: " << information.nation
       << " \n出生日期: " << information.birthday << " \n家庭住址: " << information.address << " \n身份证号: "
       << information.idNumber << " \n签发机关: " << information.signingOrganization
       << " \n有效起始日期: " << information.validityPeriodStart << " \n有效结束日期: "
       << information.validityPeriodEnd << " \n最新住址: " << information.newestAddress;
    return os;
}
QString Information::toString()
{
    QString idInfo;
    string str = "{name: " + name + ", sex: " + sex + ", nation: " + nation
                        + ", birthday: " + birthday + ", address: " + address + ", idNumber: "
                        + idNumber + " signingOrganization: " + signingOrganization
                        + ", validityPeriodStart: " + validityPeriodStart + ", validityPeriodEnd: "
                        + validityPeriodEnd + ", newestAddress: " + newestAddress+"}";
    idInfo = QString::fromStdString(str);
    return idInfo;
}


