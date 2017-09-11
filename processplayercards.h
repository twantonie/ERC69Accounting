#ifndef PROCESSPLAYERCARDS_H
#define PROCESSPLAYERCARDS_H

#include <QWidget>
#include <QVector>
#include "moneybirdapi.h"
#include "contacts.h"
#include "salesinvoice.h"
#include "playercard.h"

namespace Ui {
class ProcessPlayerCards;
}

class ProcessPlayerCards : public QWidget
{
    Q_OBJECT

public:
	explicit ProcessPlayerCards();
	~ProcessPlayerCards();

private slots:
	void on_createInvoice_pressed();
	void on_previous_pressed();
	void on_next_pressed();
	void on_find_pressed();

	void contactsReady();
	void contactSelected(int index);

	void on_dutchButton_toggled(bool checked);

	void invoicePosted();
	void invoiceSent();

private:
	void populateWindow(int page);
	void disableButtons();

	QVector<PlayerCard*> unprocessedPlayerCards;

	int currentPage;
	int selectedContactIndex = -1;

	Ui::ProcessPlayerCards *ui;
	GetQueryContacts *getContacts;
	SalesInvoice *invoice;
	ShowNameList *contactList;

	PostInvoice *post;
	SendInvoice *send;
};

#endif // PROCESSPLAYERCARDS_H
