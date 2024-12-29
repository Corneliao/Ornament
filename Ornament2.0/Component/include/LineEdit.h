#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QRegularExpressionValidator>
class LineEdit : public QWidget
{
	Q_OBJECT

public:
	LineEdit(QWidget* parent);
	~LineEdit();
protected:
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* ico = Q_NULLPTR;
	QLineEdit* lineEdit = Q_NULLPTR;
	QLabel* _close = Q_NULLPTR;
signals:
	void searchFriend(const QString& userAccount);
	void textEmpty();
	void hideAddFriend();
};
