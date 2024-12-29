#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>
#include "../../../global.h"
class LineEditComponent : public QWidget
{
	Q_OBJECT
		enum TYPE { Account = 0, Password };

public:
	LineEditComponent(const QString& type_text, const int type, QWidget* parent);
	~LineEditComponent();
	QPixmap setPixmapDpi(const QSize& size, const QPixmap& pixmap);
	QString currentText()const;
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLineEdit* lineEdit = Q_NULLPTR;
	QLabel* ico = Q_NULLPTR;
	QPixmap ico_pixmap;
signals:
	void userAccountChanged(const QString& userAccount);
};
