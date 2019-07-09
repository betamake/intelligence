#ifndef COSTBASEINFO_H
#define COSTBASEINFO_H

#include <QWidget>
class costBase{
 public:
      QString getcostRnumEidt(){return this->costRnumEidt;}
      void setcostRnumEidt(const QString costRnumEidt){this->costRnumEidt = costRnumEidt;}
      QString getcostConname() {return this->costConname;}
      void setcostConname(const QString costConname){this->costConname  = costConname;}
      int getcostCono(){return this->costCono;}
      void setcostCono(const int costCono){this->costCono = costCono;}
      QString getcostHandp(){return this->costHandp;}
      void setcostHandp(const QString costHandp){this->costHandp = costHandp;}
      QString getcostRdate(){return this->costRdate;}
      void setcostRdate(const QString costRdate){this->costRdate=  costRdate;}
      QString getcostHandpd(){return this->costHandpd = costHandpd;}
      void setcostHandpd(const QString costHandpd){this->costHandpd = costHandpd;}
      QString getcostUse(){return this->costUse;}
      void setcostUse(const QString costUse){this->costUse = costUse;}
  private:
      QString costRnumEidt; //单据编号
      QString costConname; //合同名称
      int costCono;//合同编号
      QString costHandp;//经办人
      QString costRdate; // 日期
      QString costHandpd; //经办人部门
      QString costUse; //用途
};
class costBaseInfo : public QWidget
{
    Q_OBJECT
public:
    explicit costBaseInfo(QWidget *parent = 0);
//    void costBaseRead();

signals:

public slots:
private:
    costBase costBinfo;
};


#endif // COSTBASEINFO_H
