#ifndef FINANCIALMUTATIONS_H
#define FINANCIALMUTATIONS_H

#include "jsonintrepeter.h"
#include <QVector>

class FinancialMutation
{
public:
	enum FinancialAccount {BankAccount, CashBook};

	FinancialMutation(QString amount, QString description, QString date) : mAmount(amount), mDescription(description), mDate(date) {}
	FinancialMutation() {}

	static QString getFinancialAccountID(FinancialAccount);

	QString getAmount() {return mAmount;}
	QString getName() {return mName;}
	QString getDescription() {return mDescription;}
	QString getID() {return mID;}
	QString getDate() {return mDate;}
	QString getFinancialAccountID() {return mFinancialAccountID;}

	void setAmount(QString amount) {mAmount = amount;}
	void setName(QString name) {mName = name;}
	void setDescription(QString description) {mDescription = description;}
	void setID(QString id) {mID = id;}
	void setDate(QString date) {mDate = date;}
	void setFinancialAccountID(QString id) {mFinancialAccountID = id;}

private:
	QString mAmount;
	QString mName = "";
	QString mDescription;
	QString mID = "";
	QString mDate;
	QString mFinancialAccountID;

};

class GetUnprocessedFinancialMutations : public JSONIntrepeter
{
	Q_OBJECT

public:
	GetUnprocessedFinancialMutations();
	~GetUnprocessedFinancialMutations();
	void receiveParsedReply(QJsonDocument);

signals:
	void UnprocessedFinancialMutationsReady(QVector<FinancialMutation*> mutations);
};


#endif // FINANCIALMUTATIONS_H
