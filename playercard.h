#ifndef PLAYERCARD_H
#define PLAYERCARD_H

#include <QVector>

class PlayerCard
{
public:
	enum Cards {Senior, Recreant, Colt, Junior, Cub, Mini, Benjamin, Turf, Ten, Nothing};

	PlayerCard() {}
	PlayerCard(QString firstName, QString lastName, Cards card) : mFirstName(firstName), mLastName(lastName), mCard(card) {}

	QString getFirstName(){return mFirstName;}
	QString getLastName(){return mLastName;}
	Cards getCard(){return mCard;}

	static QVector<PlayerCard*> importPlayerCardsFromExcel(QString fileName);
	static const char * getCardString(int enumVal){return CardStrings[enumVal];}

	QString getAmount();
	QString getDutchDescription();
	QString getEnglishDescription();
	QString getDutchMessage(bool sepa);
	QString getEnglishMessage(bool sepa);



private:
	static const char * CardStrings[];

	QString mFirstName;
	QString mLastName;
	Cards mCard;
};

#endif // PLAYERCARD_H
