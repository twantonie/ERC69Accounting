#include "processplayercards.h"
#include "ui_processplayercards.h"

#include <QUrl>
#include <QtWidgets>
#include <QVector>
#include <QMessageBox>
#include <QFileDialog>


QString getWorkflowID(bool dutch, bool sepa)
{
	if(dutch)
	{
		if(sepa)
			return "157733989993416688";
		else
			return "157734007082059549";
	}
	else
	{
		if(sepa)
			return "160863421165733131";
		else
			return "160863195051853566";
	}
}

void ProcessPlayerCards::disableButtons()
{
	ui->createInvoice->setEnabled(false);
	ui->previous->setEnabled(false);
	ui->next->setEnabled(false);
}

ProcessPlayerCards::ProcessPlayerCards() :
	ui(new Ui::ProcessPlayerCards)
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select player card file", "C:/Users/Twan Spil/Google Drive/Bestuur Rugby/Financien"
													, "Excel files (*.xls *.xlsx *.xlsm *.xlsb)");

	if (fileName.isNull())
	{
		QMessageBox msg;
		msg.setText("Didn't select file");
		msg.exec();
		this->deleteLater();
		return;
	}

	unprocessedPlayerCards = PlayerCard::importPlayerCardsFromExcel(fileName);

	if (unprocessedPlayerCards.size() == 0)
	{
		QMessageBox msg;
		msg.setText("Excel file did not contain player cards \n Did you select the right file?");
		msg.exec();
		this->deleteLater();
		return;
	}

	ui->setupUi(this);
	disableButtons();

	currentPage = 1;
	populateWindow(currentPage);

}

ProcessPlayerCards::~ProcessPlayerCards()
{
	delete ui;
}

void ProcessPlayerCards::on_createInvoice_pressed()
{
	disableButtons();
	ui->find->setEnabled(false);

	Contacts contact = Contacts::queriedContacts[selectedContactIndex];

	invoice = new SalesInvoice(&contact, ui->descriptionEdit->text(), ui->amountEdit->text());
	invoice->setWorkflowID(getWorkflowID(ui->dutchButton->isChecked(), contact.getSepaActive()));
	invoice->setLedgerID("165490552346773088");

	post = new PostInvoice(invoice);
	connect(post, SIGNAL(invoicePosted()), this, SLOT(invoicePosted()));
}

void ProcessPlayerCards::invoicePosted()
{
	delete post;

	send = new SendInvoice(invoice, "Email", ui->messageEdit->document()->toPlainText());
	connect(send, SIGNAL(invoiceSent()), this, SLOT(invoiceSent()));
}

void ProcessPlayerCards::invoiceSent()
{
	delete send;
	delete invoice;

	int size = unprocessedPlayerCards.size();

	unprocessedPlayerCards.remove(currentPage - 1);

	int page;
	if (currentPage == size)
		page = currentPage - 1;
	else
		page = currentPage;

	populateWindow(page);
}

void ProcessPlayerCards::on_previous_pressed()
{
	populateWindow(currentPage - 1);
}

void ProcessPlayerCards::on_next_pressed()
{
	populateWindow(currentPage + 1);
}

void ProcessPlayerCards::on_find_pressed()
{
	ui->find->setEnabled(false);
	getContacts = new GetQueryContacts(ui->nameEdit->text());
	connect(getContacts, SIGNAL(queriedContactsReceived()), this, SLOT(contactsReady()));
}

void ProcessPlayerCards::contactsReady()
{
	ui->find->setEnabled(true);

	if (currentPage != 1)
		ui->previous->setEnabled(true);

	if (currentPage != unprocessedPlayerCards.size())
		ui->next->setEnabled(true);

	delete getContacts;

	contactList = new ShowNameList();
	connect(contactList, SIGNAL(contactChosen(int)), this, SLOT(contactSelected(int)));
}

void ProcessPlayerCards::contactSelected(int index)
{
	selectedContactIndex = index;

	Contacts contact = Contacts::queriedContacts[index];

	ui->nameSelected->setText(contact.getName());

	if (contact.getSepaActive())
	{
		if(ui->dutchButton->isChecked())
			ui->messageEdit->setText(unprocessedPlayerCards[currentPage - 1]->getDutchMessage(true));
		else
			ui->messageEdit->setText(unprocessedPlayerCards[currentPage - 1]->getEnglishMessage(true));
	}
	else
	{
		if(ui->dutchButton->isChecked())
			ui->messageEdit->setText(unprocessedPlayerCards[currentPage - 1]->getDutchMessage(false));
		else
			ui->messageEdit->setText(unprocessedPlayerCards[currentPage - 1]->getEnglishMessage(false));
	}

	ui->createInvoice->setEnabled(true);

	delete contactList;
}

void ProcessPlayerCards::populateWindow(int page)
{
	int pages = unprocessedPlayerCards.size();

	ui->find->setEnabled(true);

	if (page > pages || page < 1)
	{
		QMessageBox msg;
		msg.setText("All player cards have been processed");
		msg.setWindowTitle("");
		msg.exec();
		this->deleteLater();
		return;
	}

	PlayerCard *card = unprocessedPlayerCards[page - 1];

	ui->amountEdit->setText(card->getAmount());
	ui->descriptionEdit->setText(card->getDutchDescription());
	ui->nameLabel->setText(card->getFirstName() + " " + card->getLastName());
	ui->messageEdit->setPlainText(card->getDutchMessage(true));

	ui->nameEdit->setText(card->getLastName());

	ui->page->setText(QStringLiteral("page %1 of %2").arg(page).arg(pages));

	disableButtons();

	currentPage = page;	

	on_find_pressed();
}

void ProcessPlayerCards::on_dutchButton_toggled(bool checked)
{
	bool sepa;
	if (selectedContactIndex == -1)
		sepa = true;
	else
		sepa = Contacts::queriedContacts[selectedContactIndex].getSepaActive();

	if(checked)
	{
		ui->descriptionEdit->setText(unprocessedPlayerCards[currentPage - 1]->getDutchDescription());
		ui->messageEdit->setText(unprocessedPlayerCards[currentPage - 1]->getDutchMessage(sepa));
	} else {
		ui->descriptionEdit->setText(unprocessedPlayerCards[currentPage - 1]->getEnglishDescription());
		ui->messageEdit->setText(unprocessedPlayerCards[currentPage - 1]->getEnglishMessage(sepa));
	}
}
