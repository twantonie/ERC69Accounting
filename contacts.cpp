#include "contacts.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QVBoxLayout>

QVector<Contacts> Contacts::queriedContacts;

QStringList Contacts::getQueriedContacts()
{
	QStringList list;
	for (int i = 0; i < queriedContacts.size(); i++)
	{
		list.append(queriedContacts[i].getName());
	}
	return list;
}

GetQueryContacts::GetQueryContacts(QString query)
{
	moneybird->GET("contacts.json?per_page=5&query=" + query);
}

GetQueryContacts::~GetQueryContacts(){}

void GetQueryContacts::receiveParsedReply(QJsonDocument reply)
{
	Contacts::queriedContacts.clear();

	QJsonArray mutationArray = reply.array();

	for(int i; i < mutationArray.size(); i++)
	{
		QJsonObject mutation = mutationArray[i].toObject();
		Contacts contact;

		contact.setFirstName(mutation["firstname"].toString());
		contact.setLastName(mutation["lastname"].toString());
		contact.setID(mutation["id"].toString());
		contact.setSepaActive(mutation["sepa_active"].toBool());

		Contacts::queriedContacts.append(contact);
	}

	emit queriedContactsReceived();
}

ShowNameList::ShowNameList()
{
	setWindowTitle("Select");
	button = new QPushButton("Select");

	connect(button, SIGNAL(pressed()), this, SLOT(on_select_pressed()));

	list = new QListWidget();

	list->addItems(Contacts::getQueriedContacts());
	list->setCurrentRow(0);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(list);
	layout->addWidget(button);
	setLayout(layout);

	if (list->count() > 0)
		this->show();
	else
	{
		QMessageBox msg;
		msg.setText("No contacts found");
		msg.setWindowTitle("");
		msg.exec();
	}
}

void ShowNameList::on_select_pressed()
{
	emit contactChosen(list->currentRow());
}
