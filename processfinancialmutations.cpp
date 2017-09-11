#include "processfinancialmutations.h"
#include "ui_processfinancialmutations.h"

#include <QUrl>
#include <QtWidgets>
#include <QVector>
#include <QMessageBox>


ProcessFinancialMutations::ProcessFinancialMutations() :
	ui(new Ui::ProcessFinancialMutations)
{
	getUFM = new GetUnprocessedFinancialMutations();
	connect(getUFM, SIGNAL(UnprocessedFinancialMutationsReady(QVector<FinancialMutation*>)), this, SLOT(financialMutationsReady(QVector<FinancialMutation*>)));
}

void ProcessFinancialMutations::financialMutationsReady(QVector<FinancialMutation*> mutations)
{
	delete getUFM;

	mMutations = mutations;

	if (mMutations.size() == 0)
	{
		QMessageBox msg;
		msg.setText("No more unprocessed financial mutations");
		msg.exec();
		this->deleteLater();
		return;
	}

	ui->setupUi(this);
	ui->find->setEnabled(true);
	disableButtons();

	indexMutations = 0;
	populateWindow(indexMutations);
}

void ProcessFinancialMutations::populateWindow(int page)
{
	int pages = mMutations.size();

	ui->find->setEnabled(true);

	if (page > pages || page < 0)
	{
		QMessageBox msg;
		msg.setText("No more unprocessed financial mutations");
		msg.setWindowTitle("");
		msg.exec();
		this->deleteLater();
		return;
	}

	FinancialMutation *mutation = mMutations[page];

	ui->amountEdit->setText(mutation->getAmount());
	ui->descriptionEdit->setText(mutation->getDescription());
	ui->nameLabel->setText(mutation->getName());

	ui->nameEdit->setText(mutation->getName().section(" ",-1));

	ui->page->setText(QStringLiteral("page %1 of %2").arg(page + 1).arg(pages));

	disableButtons();

	indexMutations = page;

	on_find_pressed();
}

void ProcessFinancialMutations::on_find_pressed()
{
	ui->find->setEnabled(false);
	getContacts = new GetQueryContacts(ui->nameEdit->text());
	connect(getContacts, SIGNAL(queriedContactsReceived()), this, SLOT(contactsReady()));
}

void ProcessFinancialMutations::contactsReady()
{
	ui->find->setEnabled(true);

	if (indexMutations > 0)
		ui->previous->setEnabled(true);

	if (indexMutations < mMutations.count() - 1)
		ui->next->setEnabled(true);

	delete getContacts;

	contactList = new ShowNameList();
	connect(contactList, SIGNAL(contactChosen(int)), this, SLOT(contactSelected(int)));
}

void ProcessFinancialMutations::contactSelected(int index)
{
	ui->nameSelected->setText(Contacts::queriedContacts[index].getName());

	indexContact = index;

	ui->createInvoice->setEnabled(true);

	delete contactList;
}

ProcessFinancialMutations::~ProcessFinancialMutations()
{
	delete ui;
}

void ProcessFinancialMutations::on_createInvoice_pressed()
{
	disableButtons();
	ui->find->setEnabled(false);

	invoice = new SalesInvoice(&Contacts::queriedContacts[indexContact],
							   ui->descriptionEdit->text(),
							   ui->amountEdit->text());
	invoice->setWorkflowID("157734007082059549");
	invoice->setLedgerID("157733997240124723");

	post = new PostInvoice(invoice);
	connect(post, SIGNAL(invoicePosted()), this, SLOT(invoicePosted()));
}

void ProcessFinancialMutations::invoicePosted()
{
	delete post;

	send = new SendInvoice(invoice, "Manual", "");
	connect(send, SIGNAL(invoiceSent()), this, SLOT(invoiceSent()));
}

void ProcessFinancialMutations::invoiceSent()
{
	delete send;

	link = new LinkInvoice(invoice, mMutations[indexMutations]);
	connect(link, SIGNAL(invoiceLinked()), this, SLOT(invoiceLinked()));
}

void ProcessFinancialMutations::invoiceLinked()
{
	delete link;
	delete invoice;

	int size = mMutations.size();

	mMutations.remove(indexMutations);

	int page;
	if (indexMutations == size - 1)
		page = indexMutations - 1;
	else
		page = indexMutations;

	populateWindow(page);
}

void ProcessFinancialMutations::on_previous_pressed()
{
	populateWindow(indexMutations - 1);
}

void ProcessFinancialMutations::on_next_pressed()
{
	populateWindow(indexMutations + 1);
}

void ProcessFinancialMutations::disableButtons()
{
	ui->createInvoice->setEnabled(false);
	ui->previous->setEnabled(false);
	ui->next->setEnabled(false);
}
