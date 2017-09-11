#include "startscreen.h"
#include "ui_startscreen.h"
#include "jsonintrepeter.h"
#include "processplayercards.h"
#include "processfinancialmutations.h"

StartScreen::StartScreen(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StartScreen)
{
	ui->setupUi(this);

	ui->tabWidget->clear();
	ui->tabWidget->addTab(JSONIntrepeter::getAPIToCreateWidget(), "Log");
	ui->tabWidget->setTabsClosable(true);
	connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	this->showMaximized();
}

StartScreen::~StartScreen()
{
	delete ui;
}

void StartScreen::on_actionUnprocessed_Financial_Mutations_triggered()
{
	ProcessFinancialMutations *mutations = new ProcessFinancialMutations();
	ui->tabWidget->insertTab(0, mutations, "Mutations");
	ui->tabWidget->setCurrentIndex(0);
}

void StartScreen::on_actionProcess_Player_Cards_triggered()
{
	ProcessPlayerCards *cards = new ProcessPlayerCards();
	ui->tabWidget->insertTab(0, cards, "Cards");
	ui->tabWidget->setCurrentIndex(0);
}

void StartScreen::closeTab(int index)
{
	if (index != ui->tabWidget->count() - 1)
		ui->tabWidget->removeTab(index);
}
