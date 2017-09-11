#include "playercard.h"

#include <QAxObject>

const char * PlayerCard::CardStrings[] = {"Senior", "Recreant", "Colt", "Junior", "Cub", "Mini", "Benjamin", "Turf", "Tientjeslid", ""};

QVector<PlayerCard*> PlayerCard::importPlayerCardsFromExcel(QString fileName)
{
	QVector<PlayerCard*> playerCards;

	QAxObject* excel = new QAxObject( "Excel.Application", 0 );
	QAxObject* workbooks = excel->querySubObject( "Workbooks" );
	QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName);
	QAxObject* sheets = workbook->querySubObject( "Worksheets" );

	//sheet pointer
	QAxObject* sheet = sheets->querySubObject( "Item( int )", 1 );
	QAxObject* usedrange = sheet->querySubObject("UsedRange");

	QAxObject* rows = usedrange->querySubObject( "Rows" );
	QAxObject* columns = usedrange->querySubObject( "Columns" );

	int rowStart = usedrange->property("Row").toInt();
	int colStart = usedrange->property("Column").toInt();
	int colCount = columns->property("Count").toInt();
	int rowCount = rows->property("Count").toInt();

	int lastNameCol, firstNameCol, statusCol;

	for (int col = colStart; col <= colCount + colStart; col++)
	{
		QAxObject* cell = sheet->querySubObject( "Cells( int, int )", 1, col );
		QVariant value = cell->dynamicCall( "Value()" );
		QString category = value.toString();

		if (category == "Achter naam")
			lastNameCol = col;
		else if (category == "Naam")
			firstNameCol = col;
		else if (category == "Status")
			statusCol = col;
	}

	for (int row=rowStart; row <= rowCount + rowStart; row++)
	{
			QAxObject* cellLN = sheet->querySubObject( "Cells( int, int )", row, lastNameCol );
			QAxObject* cellFN = sheet->querySubObject( "Cells( int, int )", row, firstNameCol );
			QAxObject* cellS = sheet->querySubObject( "Cells( int, int )", row, statusCol );

			QVariant valueLN = cellLN->dynamicCall( "Value()" );
			QVariant valueFN = cellFN->dynamicCall( "Value()" );
			QVariant valueS = cellS->dynamicCall( "Value()" );

			QString status = valueS.toString();
			Cards card;

			if (status == "SE - HEREN" || status == "SE - DAME")
				card = Cards::Senior;
			else if (status == "RE - HEREN" || status == "RE - DAME")
				card = Cards::Recreant;
			else if (status == "TIENTJESLID")
				card = Cards::Ten;
			else if (status == "CO - JONGEN" || status == "CO - MEISJE")
				card = Cards::Colt;
			else if (status == "JU - JONGEN" || status == "JU - MEISJE")
				card = Cards::Junior;
			else if (status == "CU - JONGEN" || status == "CU - MEISJE")
				card = Cards::Cub;
			else if (status == "MI - JONGEN" || status == "MI - MEISJE")
				card = Cards::Mini;
			else if (status == "BE - JONGEN" || status == "BE - MEISJE")
				card = Cards::Benjamin;
			else if (status == "TU - JONGEN" || status == "TU - MEISJE")
				card = Cards::Turf;
			else
				card = Cards::Nothing;

			if (card != Cards::Nothing)
			{
				PlayerCard *playerCard = new PlayerCard(valueFN.toString(), valueLN.toString(), card);
				playerCards.append(playerCard);
			}
	}

	workbook->dynamicCall("Close()");
	excel->dynamicCall("Quit()");
	return playerCards;
}

QString PlayerCard::getAmount()
{
	switch(mCard)
	{
	case Cards::Senior : return "67";
	case Cards::Recreant : return "34";
	case Cards::Ten : return "10";
	case Cards::Colt : return "50";
	case Cards::Junior : return "29";
	case Cards::Cub :
	case Cards::Mini : return "23";
	case Cards::Benjamin :
	case Cards::Turf : return "18";
	default : return "";
	}
}

QString PlayerCard::getDutchDescription()
{
	QString description = "Spelerskaart ";
	description.append(getCardString(mCard));
	description.append(" (" + mFirstName + ")\n");
	description.append("Seizoen 2017-2018");

	return description;
}

QString PlayerCard::getEnglishDescription()
{
	QString description = "Dutch rugby union contribution (Spelerskaart) ";
	description.append(getCardString(mCard));
	description.append(" (" + mFirstName + ")\n");
	description.append("Season 2017-2018");

	return description;
}

QString PlayerCard::getDutchMessage(bool sepa)
{

	QString message = "Beste ";
	if (!(mCard==Cards::Senior || mCard==Cards::Recreant || mCard==Cards::Ten))
	{
		message.append("ouder van ");
	}
	message.append(mFirstName + ",\n\n");
	message.append("In de bijlage kunt u de factuur vinden voor de spelerskaart. ");

	if (sepa)
		message.append("Dit bedrag wordt binnenkort van uw rekening afgeschreven. \n\n");
	else
		message.append("We verzoeken u vriendelijk dit bedrag over te maken naar NL59ABNA0405115245 t.n.v ERC'69. \n\n");

	message.append("Met vriendelijke groet, \n");
	message.append("Twan Spil \n");
	message.append("Penningmeester ERC'69");

	return message;
}

QString PlayerCard::getEnglishMessage(bool sepa)
{
	QString message = "Dear ";
	if (!(mCard==Cards::Senior || mCard==Cards::Recreant || mCard==Cards::Ten))
	{
		message.append("parent of ");
	}
	message.append(mFirstName + ",\n\n");
	message.append("In the attachment you can find the invoice for membership of the Dutch rugby union. ");

	if(sepa)
		message.append("This amount will be collected from your bank account in the coming week. \n\n");
	else
		message.append("Could you please transfer this money to NL59ABNA0405115245 in the name of ERC'69. \n\n");

	message.append("With kind regards, \n");
	message.append("Twan Spil \n");
	message.append("Treasurer ERC'69");

	return message;
}
