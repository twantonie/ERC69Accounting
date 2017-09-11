#include "salesinvoice.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <playercard.h>


QJsonDocument PostInvoice::parseInvoiceToJson()
{
	QJsonObject detailsJson;

	for (int i=0; i < mInvoice->getDetailsSize(); i++)
	{
		QJsonObject detailsRowJson;
		detailsRowJson["price"] = mInvoice->getAmount(i);
		detailsRowJson["description"] = mInvoice->getDescription(i);
		addToJsonObject(&detailsRowJson, "ledger_account_id", mInvoice->getLedgerID(i));

		detailsJson[QString::number(i)] = detailsRowJson;
	}

	QJsonObject invoiceJson;
	invoiceJson["contact_id"] = mInvoice->getContact()->getID();
	addToJsonObject(&invoiceJson, "workflow_id", mInvoice->getWorkflowID());
	invoiceJson["details_attributes"] = detailsJson;

	QJsonObject sales_invoice;
	sales_invoice["sales_invoice"] = invoiceJson;

	QJsonDocument done(sales_invoice);
	return done;
}

PostInvoice::PostInvoice(SalesInvoice *invoice) : mInvoice(invoice)
{
	moneybird->POST("sales_invoices.json", parseInvoiceToJson());
}

void PostInvoice::receiveParsedReply(QJsonDocument reply)
{
	QJsonObject obj = reply.object();
	mInvoice->setInvoiceID(obj["id"].toString());
	mInvoice->setTotalUnpaid(obj["total_unpaid"].toString());

	emit invoicePosted();
}

SendInvoice::SendInvoice(SalesInvoice *invoice, QString deliveryMethod, QString message) : mInvoice(invoice)
{
	QString url = "sales_invoices/" + mInvoice->getInvoiceID() + "/send_invoice.json";

	QJsonObject invoiceSending;
	invoiceSending["delivery_method"] = deliveryMethod;
	invoiceSending["email_message"] =  message;

	QJsonObject doneObject;
	doneObject["sales_invoice_sending"] = invoiceSending;

	QJsonDocument done(doneObject);

	moneybird->PATCH(url, done);
}

void SendInvoice::receiveParsedReply(QJsonDocument reply)
{
	if (reply.isNull())
		emit invoiceSent();
}

LinkInvoice::LinkInvoice(SalesInvoice *invoice, FinancialMutation *mutation) : mInvoice(invoice), mMutation(mutation)
{
	QString url = "sales_invoices/" + mInvoice->getInvoiceID() + "/register_payment.json";

	QJsonObject payment;
	payment["payment_date"] = QDateTime::currentDateTime().toString(Qt::ISODate);
	payment["price"] = mInvoice->getTotalUnpaid();
	payment["financial_account_id"] = mutation->getFinancialAccountID();
	payment["financial_mutation_id"] = mutation->getID();

	QJsonObject done;
	done["payment"] = payment;

	QJsonDocument data(done);

	moneybird->PATCH(url, data);
}

void LinkInvoice::receiveParsedReply(QJsonDocument reply)
{
	if (!reply.isNull())
		emit invoiceLinked();
}
