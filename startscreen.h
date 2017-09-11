#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QMainWindow>

namespace Ui {
class StartScreen;
}

class StartScreen : public QMainWindow
{
	Q_OBJECT

public:
	explicit StartScreen(QWidget *parent = 0);
	~StartScreen();

private slots:
	void on_actionUnprocessed_Financial_Mutations_triggered();
	void on_actionProcess_Player_Cards_triggered();
	void closeTab(int index);

private:
	Ui::StartScreen *ui;
};

#endif // STARTSCREEN_H
