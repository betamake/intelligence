//
// Created by Dragic on 2018/5/22.
//

#ifndef IDCARDREADER_TIMESTAMP_H
#define IDCARDREADER_TIMESTAMP_H
//时间戳类型
typedef long long timestamp_t;

class Timestamp {
public:
    //获取以毫秒计的时间戳
    static timestamp_t now();
};


#endif //IDCARDREADER_TIMESTAMP_H
