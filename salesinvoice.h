#ifndef SALESINVOICE_H
#define SALESINVOICE_H

#include "jsonintrepeter.h"
#include "contacts.h"
#include "financialmutations.h"
#include <QJsonDocument>

class SalesInvoice
{
	class InvoiceDetail
	{
	public:
		InvoiceDetail(QString description, QString amount) : mDescription(description), mAmount(amount) {}
		InvoiceDetail() {}

		QString getDescription() {return mDescription;}
		QString getAmount() {return mAmount;}
		QString getLedgerID() {return mLedgerID;}

		void setLedgerID(QString ledgerID) {mLedgerID = ledgerID;}
		void setAmount(QString amount) {mAmount = amount;}
		void setDescription(QString description) {mDescription = description;}

	private:
		QString mDescription;
		QString mAmount;
		QString mLedgerID;
	};

public:
	SalesInvoice(Contacts *contact, QString description, QString amount) : mContact(contact) {details.append(InvoiceDetail(description, amount));}

	void addDetail(QString description, QString amount) {details.append(InvoiceDetail(description, amount));}

	void setContact(Contacts *contact) {mContact = contact;}
	void setWorkflowID(QString workflowID) {mWorkflowID = workflowID;}
	void setInvoiceID(QString id) {mInvoiceID = id;}
	void setTotalUnpaid(QString unpaid) {mTotalUnpaid = unpaid;}
	void setLedgerID(QString id, int index = 0) {details[index].setLedgerID(id);}
	void setDescription(QString description, int index = 0) {details[index].setDescription(description);}
	void setAmount(QString amount, int index = 0) {details[index].setAmount(amount);}

	Contacts* getContact() {return mContact;}
	QString getWorkflowID() {return mWorkflowID;}
	QString getInvoiceID() {return mInvoiceID;}
	QString getTotalUnpaid() {return mTotalUnpaid;}
	QString getLedgerID(int index = 0) {return details[index].getLedgerID();}
	QString getAmount(int index = 0) {return details[index].getAmount();}
	QString getDescription(int index = 0) {return details[index].getDescription();}

	int getDetailsSize() {return details.size();}

private:
	Contacts *mContact;
	QVector<InvoiceDetail> details;
	QString mWorkflowID;
	QString mInvoiceID = "";
	QString mTotalUnpaid = "";


};

class PostInvoice : public JSONIntrepeter
{
	Q_OBJECT

public:
	PostInvoice(SalesInvoice *invoice);
	void receiveParsedReply(QJsonDocument);

private:
	SalesInvoice *mInvoice;
	QJsonDocument parseInvoiceToJson();

signals:
	void invoicePosted();
};

class SendInvoice : public JSONIntrepeter
{
	Q_OBJECT

public:
	SendInvoice(SalesInvoice *invoice, QString deliveryMethod, QString message);
	void receiveParsedReply(QJsonDocument);

private:
	SalesInvoice *mInvoice;

signals:
	void invoiceSent();
};

class LinkInvoice : public JSONIntrepeter
{
	Q_OBJECT

public:
	LinkInvoice(SalesInvoice *invoice, FinancialMutation *mutation);
	void receiveParsedReply(QJsonDocument);

private:
	SalesInvoice *mInvoice;
	FinancialMutation *mMutation;

signals:
	void invoiceLinked();

};



#endif // SALESINVOICE_H
