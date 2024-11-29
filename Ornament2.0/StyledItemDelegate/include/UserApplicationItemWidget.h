#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QEvent>
#include "../../Component/include/RoundImage.h"
#include "../../global.h"
class UserApplicationButton;
class UserApplicationItemWidget : public QWidget
{
	Q_OBJECT

public:
	UserApplicationItemWidget(const UserData& user_data, QWidget* parent);
	~UserApplicationItemWidget();
protected:
	bool eventFilter(QObject* target, QEvent* event) Q_DECL_OVERRIDE;
private:
	UserData m_userData;
	QLabel* userHead = Q_NULLPTR;
	QLabel* userName = Q_NULLPTR;
	QLabel* application = Q_NULLPTR;
	UserApplicationButton* agree = Q_NULLPTR;
	UserApplicationButton* disAgree = Q_NULLPTR;
signals:
	void agreenApplicationSignal(const QString cronyAccount);
	void disAgreeApplicationSignal();
};

class UserApplicationButton :public QWidget
{
public:
	explicit UserApplicationButton(const QString& button_text, QWidget* parent = Q_NULLPTR);
	~UserApplicationButton();
public:
	void setBackgroundColor(const QColor& color);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* buttonText = Q_NULLPTR;
	QColor backgroundColor = QColor::fromString("#87CEFA");
};
