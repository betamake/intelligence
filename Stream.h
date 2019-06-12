//
// Created by Dragic on 2018/5/22.
//

#ifndef IDCARDREADER_STREAM_H
#define IDCARDREADER_STREAM_H


class Stream {
private:
    //缓冲区
    char* buffer;
    //缓冲区容量
    int capacity;
    //流的开头
    int start;
    //流的长度
    int length;

public:
    //初始化一个流，参数为：初始容量
    Stream(int initCapacity=16);
    //清理
    ~Stream();
    //获取流的长度
    int getLength();
    //向流的末尾增加一字符
    void append(char aChar);
    //向流的末尾增加多个字符
    void append(const char* buf,int len);
    //查看流的第一个字符，如果长度为0则返回0
    char peek();
    //查看流开头的多个字符，返回实际查看到的长度
    int peek(char* buf,int len);
    //取出流的第一个字符，如果长度为0则返回0
    char take();
    //取出流开头的多个字符，返回实际取出的长度
    int take(char* buf,int len);

private:
    //扩大容量至两倍
    void expand();
};


#endif //IDCARDREADER_STREAM_H
