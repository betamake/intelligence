#ifndef POSTDATA_H
#define POSTDATA_H

#include <QObject>
typedef struct {
    int projectCode; //预算项目号
    QString projectName; //预算项目名称
    QString moneyOrigin; //经费类型
    QString expensesProjectName; //业务事项
    QString itemCode; //预算科目
    QString moneyRemi;//报销金额
    QString departCenterName;//使用科室

}detalList;
typedef struct {
    QString payState;//支付状态
    QString offset;//是否冲借款
    QString qkDetailId;//借款单
    QString moneyLoan;//借款金额
    QString paymethod; //支付方式
    QString moneyRemi; //报销金额
    QString tradeDate; //公务卡交易日期
    QString counterPartName; //对方单位名称
    QString moneyCard; //公务卡交易金额
    QString collectPeople; //收款人单位
    QString bankName; //所属银行
    QString bankProvince; //省
    QString bankCity;//市
    QString bankAddress; //具体开户行
    QString bankNo; //银行账号
    QString payToken; //支付口令
    QString payUse;//汇款用途
    QString memo; //备注

}payList;
class SetCostData{
public:
    int getid()
    {
        return this->id;
    }
    void setid(const int id ){
        this->id = id;
    }
    QString getcode(){
        return this->code;
    }
    void setcode(const QString code){
        this->code =code;
    }
    QString getbillDate(){
        return this->billDate;
    }
    void setbillDate(const QString billDate){
        this->billDate =billDate;
    }
    QString getoperater(){
        return this->operater;
    }
    void setoperater(const QString operater){
        this->operater=operater;
    }
    QString getdepartId(){
        return this->departId;
    }
    void setdepartId(const QString departId){
        this->departId=departId;
    }
    QString getuse()
    {
        return this->use;
    }
    void setuse(const QString use){
        this->use=use;
    }
    QString getotherText(){
        return this->otherText;
    }
    void setotherText(const QString otherText){
        this->otherText=otherText;
    }
    QString getleader(){
        return this->leader;
    }
    void setleader(const QString leader){
        this->leader=leader;
    }
    QString getleaderDownList(){
        return this->leaderDownList;
    }
    void setleaderDownList(const QString leaderDownList){
        this->leaderDownList=leaderDownList;
    }
    QString getmoneyRemi(){
        return this->moneyRemi;
    }
    void setmoneyRemi(const QString moneyRemi){
        this->moneyRemi=moneyRemi;
    }
    QString getcontractId(){
        return this->contractId;
    }
    void setcontractId(const QString contractId){
        this->contractId=contractId;
    }
    QString getcontractCode(){
        return this->contractCode;
    }
    void setcontractCode(const QString contractCode){
        this->contractCode=contractCode;
    }
    QString getcontractName(){
        return this->contractName;
    }
    void setcontractName(const QString contractName){
        this->contractName=contractName;
    }
    QString getmoneyContractCode(){
        return this->moneyContractCode;
    }
    void setmoneyContractCode(const QString moneyContractCode){
        this->moneyContractCode=moneyContractCode;
    }
    QString getisHaveJk(){
        return this->isHaveJk;
    }
    void  setisHaveJk(const QString isHaveJk){
        this->isHaveJk=isHaveJk;
    }
    QString getmoneyLoan(){
        return this->moneyLoan;
    }
    void setmoneyLoan(const QString moneyLoan){
        this->moneyLoan=moneyLoan;
    }
    QString getmoneyOffset(){
        return this->moneyOffset;
    }
    void setmoneyOffset(const QString moneyOffset){
        this->moneyOffset=moneyOffset;
    }
    QString getmoneyReissue(){
        return this->moneyReissue;
    }
    void setmoneyReissue(const QString moneyReissue){
        this->moneyReissue=moneyReissue;
    }

    QString getmoneyBack(){
        return this->moneyBack;
    }
    void setmoneyBack(const QString moneyBack){
        this->moneyBack=moneyBack;
    }
    QString getpayDate(){
        return this->payDate;
    }
    void setpayDate(const QString payDate){
        this->payDate=payDate;
    }
    int getticketNo(){
        return this->ticketNo;
    }
    void setticketNo(const int ticketNo){
        this->ticketNo=ticketNo;
    }
    int getdetallistprojectCode(){
        return this->detallist.projectCode;
    }
    void setdetallistprojectCode(const int projectCode){
        this->detallist.projectCode=projectCode;
    }
    QString getdetallistprojectName(){
        return this->detallist.projectName;
    }
    void setdetallistprojectName(const QString projectName)
    {
        this->detallist.projectName = projectName;

    }
    QString getdetallistmoneyOrigin(){
        return this->detallist.moneyOrigin;
    }
    void setdetallistmoneyOrigin(const QString moneyOrigin){
        this->detallist.moneyOrigin=moneyOrigin;
            }
    QString getdetallistexpensesProjectName(){
        return this->detallist.expensesProjectName;
    }
    void setdetallistexpensesProjectName(const QString expensesProjectName){
        this->detallist.expensesProjectName=expensesProjectName;
    }
    QString getdetallistitemCode(){
        return this->detallist.itemCode;
    }
    void setdetallistitemCode(const QString itemCode)
    {
        this->detallist.itemCode = itemCode;
    }
    QString getdetallistmoneyRemi(){
        return this->detallist.moneyRemi;
    }
    void setdetallistmoneyRemi(const QString moneyRemi){
        this->detallist.moneyRemi = moneyRemi;
    }
    QString getdetallistdepartCenterName(){
        return this->detallist.departCenterName;
    }
    void setdetallistdepartCenterName(const QString departCenterName)
    {
        this->detallist.departCenterName=departCenterName;
    }

private:
    int id ; //单据id
    QString code; //单据编号
    QString billDate; //单据日期
    QString operater; //经办人
    QString departId; //接待办公室部门
    QString use; //使用用途
    QString otherText;//其他事项说明
    QString leader; //政府性指令
    QString leaderDownList; //政府性指令下拉框
    QString moneyRemi;//报销金额
    QString contractId; //合同ID
    QString contractCode;//合同编码
    QString contractName;//合同名称
    QString moneyContractCode; //财务合同编号
    QString isHaveJk; //经办人是否冲借款
    QString moneyLoan; //剩余借款金额合计
    QString moneyOffset; //冲借款金额
    QString moneyReissue; //应补发金额
    QString moneyBack; //应归还金额
    QString payDate; //支付日期
    int ticketNo;//电子发票号
    detalList detallist; //报销明细
    payList paylist; //支付信息


};
class SetTralvelData{

};
class SetAbroadData{


};

class postData : public QObject
{
    Q_OBJECT
public:
    explicit postData(QObject *parent = 0);
//    ~postData();
//    SetCostData getCostData(){
//        return costData;
//    }
//    SetCostData costData;
//    void postCostData(); //给后端传数据.

signals:

public slots:
private:
//    static postData *instance;
};

#endif // POSTDATA_H
