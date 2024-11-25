#include "../include/LineEditComponent.h"

LineEditComponent::LineEditComponent(const QString& type_text, const int type, QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	main_lay->setContentsMargins(11, 0, 11, 0);
	this->setLayout(main_lay);
	this->setFixedHeight(40);

	this->ico = new QLabel(this);
	this->ico->setFixedSize(19, 19);
	this->ico->setScaledContents(true);

	QFont font;
	font.setPixelSize(13);
	QStringList qsslist;
	qsslist.append("QLineEdit:Hover{background-color:transparent}");
	qsslist.append("QLineEdit{background-color:transparent}");
	this->lineEdit = new QLineEdit(this);
	this->lineEdit->setFont(font);
	this->lineEdit->setStyleSheet(qsslist.join(""));
	this->lineEdit->setFrame(QFrame::NoFrame);
	this->lineEdit->setAlignment(Qt::AlignCenter);
	this->lineEdit->setMaxLength(9);
	this->lineEdit->setFocusPolicy(Qt::ClickFocus);
	if (type == TYPE::Account) {
		if (type_text == "登录") {
			this->lineEdit->setPlaceholderText("输入账号");
			QRegularExpression regx("[1-9][0-9]+$");
			QValidator* validator = new QRegularExpressionValidator(regx, this);
			this->lineEdit->setValidator(validator);
			this->ico_pixmap.load(":/Resource/ico/MingcuteUser2Fill.png");
			this->ico_pixmap = this->setPixmapDpi(this->ico->size(), this->ico_pixmap);
			this->ico->setPixmap(ico_pixmap);
			connect(this->lineEdit, &QLineEdit::textChanged, this, &LineEditComponent::userAccountChanged, Qt::DirectConnection);
		}
		else if (type_text == "注册") {
			this->lineEdit->setPlaceholderText("输入昵称");
			this->ico_pixmap.load(":/Resource/ico/MingcuteUser2Fill.png");
			this->ico_pixmap = this->setPixmapDpi(this->ico->size(), this->ico_pixmap);
			this->ico->setPixmap(ico_pixmap);
		}
	}
	else if (type == TYPE::Password) {
		this->lineEdit->setPlaceholderText("输入密码");
		this->lineEdit->setEchoMode(QLineEdit::Password);
		this->ico_pixmap.load(":/Resource/ico/MajesticonsLock.png");
		this->ico_pixmap = this->setPixmapDpi(this->ico->size(), this->ico_pixmap);
		this->ico->setPixmap(ico_pixmap);
	}
	main_lay->addWidget(this->ico);
	main_lay->addWidget(this->lineEdit);
}

LineEditComponent::~LineEditComponent()
{
}

QPixmap LineEditComponent::setPixmapDpi(const QSize& size, const QPixmap& pixmap)
{
	QPixmap resultPixmap = pixmap.scaled(size * GLOB_ScaleDpi, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	return resultPixmap;
}

QString LineEditComponent::currentText() const
{
	if (this->lineEdit->text().isEmpty())
		return "";
	return this->lineEdit->text();
}

void LineEditComponent::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor::fromString("#eff2f6"));
	painter.drawRoundedRect(this->rect(), 10, 10);
}