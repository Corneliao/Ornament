#pragma once

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QListWidget>
#include "../StyledItemDelegate/include/FeatureBarDelegate.h"
#include "AddFriend.h"
class AddFriendButton;
class FeatureButton;
class ApplicationFeaureBar : public QWidget
{
	Q_OBJECT

public:
	ApplicationFeaureBar(QWidget* parent);
	~ApplicationFeaureBar();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
	bool eventFilter(QObject* target, QEvent* event) Q_DECL_OVERRIDE;
private:
	void increaseFeature(const QString& normal_ico, const QString& select_ico, const QString& ico_text);
	void dealFeatureButtonClicked(const int index);
private:
	QVBoxLayout* main_vbox = Q_NULLPTR;
	AddFriendButton* addfriend_button = Q_NULLPTR;
	QListWidget* feature_listWidget = Q_NULLPTR;
	int itemIndex = 0;
	AddFriend* addFriend = Q_NULLPTR;
signals:
	void indexChanged(const int index);
};

class AddFriendButton :public QWidget {
public:
	AddFriendButton(QWidget* parent = Q_NULLPTR);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* ico = Q_NULLPTR;
	QLabel* ico_text = Q_NULLPTR;
};
class  FeatureButton :public QWidget {
	Q_OBJECT
public:
	FeatureButton(const QString& normaIco, const QString& selectedIco, const QString& buttonText, const int index, QWidget* parent = Q_NULLPTR);
	int GetCurrentIndex()const;
	void setUnSelected();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent*)Q_DECL_OVERRIDE;
private:
	QString ico_text;
	QPixmap selectedPixmap;
	QPixmap normalPixmap;
	bool isPressed = false;
	int currentIndex = 0;
signals:
	void clicked(const int index);
};
