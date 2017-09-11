#include "jsonintrepeter.h"
#include <QThread>
#include <QException>

bool JSONIntrepeter::busy = false;
bool JSONIntrepeter::widgetCreated = false;
MoneybirdAPI *JSONIntrepeter::moneybird;

MoneybirdAPI* JSONIntrepeter::getAPIToCreateWidget()
{
	if (!widgetCreated)
		return MoneybirdAPI::create(false);
	else
		return nullptr;
}

JSONIntrepeter::JSONIntrepeter(QObject *parent) : QObject(parent)
{
	moneybird = MoneybirdAPI::create(debugOn);

	if (busy)
		throw 1;

	busy = true;

	connect(moneybird, SIGNAL(ParsedReply(QJsonDocument)),
			this, SLOT(receiveParsedReply(QJsonDocument)));
}

JSONIntrepeter::~JSONIntrepeter()
{
	busy = false;
}

void JSONIntrepeter::addToJsonObject(QJsonObject *object, const QString parameter, const QString value)
{
	if (value != "")
	{
		(*object)[parameter] = value;
	}
}
