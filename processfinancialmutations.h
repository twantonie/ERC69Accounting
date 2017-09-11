#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "moneybirdapi.h"
#include "financialmutations.h"
#include "contacts.h"
#include "salesinvoice.h"

namespace Ui {
class ProcessFinancialMutations;
}

class ProcessFinancialMutations : public QMainWindow
{
    Q_OBJECT

public:
	explicit ProcessFinancialMutations();
	~ProcessFinancialMutations();

private slots:
	void on_createInvoice_pressed();
	void on_previous_pressed();
	void on_next_pressed();
	void on_find_pressed();

	void financialMutationsReady(QVector<FinancialMutation*>);
	void contactsReady();
	void contactSelected(int index);

	void invoicePosted();
	void invoiceSent();
	void invoiceLinked();

private:
	void populateWindow(int page);
	void disableButtons();
	int indexMutations;
	int indexContact;
	QVector<FinancialMutation*> mMutations;

	Ui::ProcessFinancialMutations *ui;
	GetUnprocessedFinancialMutations *getUFM;	
	GetQueryContacts *getContacts;
	SalesInvoice *invoice;
	ShowNameList *contactList;

	PostInvoice *post;
	SendInvoice *send;
	LinkInvoice *link;
};

#endif // MAINWINDOW_H
