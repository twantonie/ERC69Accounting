#ifndef CONTACTS_H
#define CONTACTS_H

#include "jsonintrepeter.h"
#include <QVector>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>

class Contacts
{
public:
	Contacts() {}

	static QStringList getQueriedContacts();
	static QVector<Contacts> queriedContacts;

	QString getID() {return mID;}
	QString getFirstName() {return mFirstName;}
	QString getLastName() {return mLastName;}
	QString getName() {return mFirstName + " " + mLastName;}
	QString getAddress() {return mAddress;}
	QString getZipCode() {return mZipCode;}
	QString getCity() {return mCity;}
	QString getPhone() {return mPhone;}
	QString getEmail() {return mEmail;}
	bool getSepaActive() {return mSepaActive;}
	QString getIban() {return mIban;}
	QString getBIC() {return mBIC;}
	QString getMandateID() {return mMandateID;}
	QString getMandateDate() {return mMandateDate;}

	void setID(QString id) {mID = id;}
	void setFirstName(QString firstName) {mFirstName = firstName;}
	void setLastName(QString lastName) {mLastName = lastName;}
	void setAddress(QString address) {mAddress = address;}
	void setZipCode(QString zipcode) {mZipCode = zipcode;}
	void setCity(QString city) {mCity = city;}
	void setPhone(QString phone) {mPhone = phone;}
	void setEmail(QString email) {mEmail = email;}
	void setSepaActive(bool sepaActive) {mSepaActive = sepaActive;}
	void setIban(QString iban) {mIban = iban;}
	void setBic(QString bic) {mBIC = bic;}
	void setMandateID(QString mandateID) {mMandateID = mandateID;}
	void setMandateDate(QString date) {mMandateDate = date;}
private:
	QString mID = "";
	QString mFirstName = "";
	QString mLastName = "";
	QString mAddress = "";
	QString mZipCode = "";
	QString mCity = "";
	QString mPhone = "";
	QString mDeliveryMethod = "Manual";
	QString mEmail = "";
	bool mEmailUBL = false;
	bool mSepaActive = false;
	QString mIban = "";
	QString mBIC = "";
	QString mMandateID = "";
	QString mMandateDate = "";
};

class GetQueryContacts : public JSONIntrepeter
{
	Q_OBJECT

public:
	GetQueryContacts(QString query);
	~GetQueryContacts();
	void receiveParsedReply(QJsonDocument);

signals:
	void queriedContactsReceived();

};

class ShowNameList : public QWidget
{
	Q_OBJECT

public:
	ShowNameList();

private:
	QListWidget *list;
	QPushButton *button;

public slots:
	void on_select_pressed();

signals:
	void contactChosen(int index);

};

#endif // CONTACTS_H
