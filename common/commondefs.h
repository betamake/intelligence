#ifndef COMMONDEFS_H
#define COMMONDEFS_H

#include <QString>

class Date{
public:
    Date();
    Date(int y, int m, int d);

    void setYear(int y) { this->year = y;}
    void setMonth(int m) { this->month = m;}
    void setDay(int d) {this->day = d;}

    int getYear() { return this->year;}
    int getMonth() { return this->month;}
    int getDay() { return this->day;}

    QString getDate() { return QString("%1-%2-%3").arg(this->year).arg(this->month).arg(this->day);}

private:
    int year;
    int month;
    int day;
};

class commonDefs
{
public:
    commonDefs();
};

#endif // COMMONDEFS_H
