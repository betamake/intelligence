#ifndef PAYINFOMANAGER_H
#define PAYINFOMANAGER_H

#include <QObject>

class payItemInfo
{
public:
    void setType(const int type) { this->type = type;}
    void setAccount(const int account) { this->account = account;}
    void setPayeeName(const QString &payeeName) { this->payeeName = payeeName;}
    void setDepartment(const QString &department) { this->department = department;}
    void setProvince(const QString &province) { this->province = province;}
    void setCity(const QString &city) { this->city = city;}
    void setBank(const QString &bank) { this->bank = bank;}
    void setBankName(const QString &bankName) { this->bankName = bankName;}
    void setBankcardNumber(const QString &bankcardNum) { this->bankcardNumber = bankcardNum;}
    void setUsage(const QString &usage) { this->usage = usage;}
    void setRemark(const QString &remark) { this->remark = remark;}
    void setCardDealDate(const QString &cardDealDate) { this->cardDealDate = cardDealDate;}
    void setCardAccount(int cardAccount) { this->cardAccount = cardAccount;}

    void clear();

    int getType() const { return this->type;}
    int getAccount() const { return this->account;}
    QString getPayeeName() const { return this->payeeName;}
    QString getDepartment() const { return this->department;}
    QString getProvince() const { return this->province;}
    QString getCity() const { return this->city;}
    QString getBank() const { return this->bank;}
    QString getBankName() const { return this->bankName;}
    QString getBankcardNumber() const { return this->bankcardNumber;}
    QString getUsage() const { return this->usage;}
    QString getRemark() const { return this->remark;}
    QString getCardDealDate() const { return this->cardDealDate;}
    int getCardAccount() const { return this->cardAccount;}

private:
    int type;               //支付方式
    int account;            //支付金额
    QString payeeName;      //收款人姓名
    QString department;     //收款人单位
    QString province;       //省
    QString city;           //市
    QString bank;           //所属银行
    QString bankName;       //开户行名称
    QString bankcardNumber; //银行帐号
    QString usage;          //汇款用途
    QString remark;         //备注

    QString cardDealDate;   //公务卡交易日期
    int cardAccount;        //公务卡交易金额
};


class payInfoManager : public QObject
{
    Q_OBJECT
public:
    static payInfoManager *getInstance();

    explicit payInfoManager(QObject *parent = nullptr);
    ~payInfoManager();

public:
    void delCurrentItem();

    void addPayItem(payItemInfo *info);     //添加支付信息
    void deletePayItem(payItemInfo *info);
    void clearPayItem(payItemInfo *info);

    QList<payItemInfo*> getPayList();

signals:
    void deletePayItem();

    void addPayItemView(payItemInfo *info);

public slots:

private:
    static payInfoManager *instance;

    QList<payItemInfo*> payList;

    int itemNumber;         //支付信息项目总数
};

#endif // PAYINFOMANAGER_H
