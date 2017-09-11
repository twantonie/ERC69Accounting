#ifndef JSONINTREPETER_H
#define JSONINTREPETER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <moneybirdapi.h>

class JSONIntrepeter : public QObject
{
	Q_OBJECT

public:
	static MoneybirdAPI* getAPIToCreateWidget(); //One time only

protected:
	static MoneybirdAPI *moneybird;
	explicit JSONIntrepeter(QObject *parent = nullptr);
	~JSONIntrepeter();

	void addToJsonObject(QJsonObject *object, const QString parameter, const QString value);
private:
	static bool busy;
	static bool widgetCreated;
	bool debugOn = false;

public slots:
	virtual void receiveParsedReply(QJsonDocument) = 0;

};

#endif // JSONINTREPETER_H
