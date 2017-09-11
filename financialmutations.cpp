#include "financialmutations.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QString FinancialMutation::getFinancialAccountID(FinancialAccount account)
{
	switch (account) {
	case (FinancialAccount::BankAccount):
		return "157734015254660172";
	case (FinancialAccount::CashBook):
		return "158511091531383924";
	default:
		return "";
	}
}

GetUnprocessedFinancialMutations::GetUnprocessedFinancialMutations()
{
	moneybird->GET("financial_mutations.json?filter=state:unprocessed");
}

GetUnprocessedFinancialMutations::~GetUnprocessedFinancialMutations()
{

}

void GetUnprocessedFinancialMutations::receiveParsedReply(QJsonDocument reply)
{
	QJsonArray mutationArray = reply.array();
	QVector<FinancialMutation*> mutations;

	for(int i = 0; i < mutationArray.size(); i++)
	{		
		QJsonObject mutationJson = mutationArray[i].toObject();

		QString message = mutationJson["message"].toString().section("/",-3,-3);
		QString name = mutationJson["contra_account_name"].toString();


		if (name == "ABN AMRO BANK INZ TIKKIE")
		{
			name = message.section(",", 2);
			message = message.section(",", 1, 1);
		}

		FinancialMutation *mutation = new FinancialMutation(mutationJson["amount"].toString(), message, mutationJson["date"].toString());
		mutation->setName(name);
		mutation->setID(mutationJson["id"].toString());
		mutation->setFinancialAccountID(mutationJson["financial_account_id"].toString());
		mutations.append(mutation);
	}

	emit UnprocessedFinancialMutationsReady(mutations);
}




