#include "../include/LineEdit.h"

LineEdit::LineEdit(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	main_lay->setContentsMargins(0, 0, 0, 0);
	this->setLayout(main_lay);

	this->setFixedHeight(40);

	this->ico = new QLabel(this);
	this->ico->setFixedSize(15, 15);
	this->ico->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/IconamoonSearch.png");
	this->ico->setPixmap(pixmap);

	QRegularExpression regx("[1-9][0-9]+$");
	QValidator* validator = new QRegularExpressionValidator(regx, this);
	this->lineEdit = new QLineEdit(this);
	this->lineEdit->setValidator(validator);
	this->lineEdit->setClearButtonEnabled(true);
	this->lineEdit->setFrame(QFrame::NoFrame);
	this->lineEdit->setAttribute(Qt::WA_Hover);
	this->lineEdit->installEventFilter(this);
	QStringList qsslist;
	qsslist.append("QLineEdit:hover{background-color:transparent}");
	qsslist.append("QLineEdit{background-color:transparent}");
	this->lineEdit->setStyleSheet(qsslist.join(""));
	this->lineEdit->setPlaceholderText("输入用户账号或者用户昵称");

	this->_close = new QLabel(this);
	this->_close->setFixedSize(20, 20);
	this->_close->setCursor(Qt::PointingHandCursor);
	this->_close->setScaledContents(true);
	pixmap.load(":/Resource/ico/IconamoonClose.png");
	this->_close->setPixmap(pixmap);
	this->_close->installEventFilter(this);
	this->_close->installEventFilter(this);

	main_lay->addWidget(this->ico);
	main_lay->addWidget(this->lineEdit);
	main_lay->addWidget(this->_close);

	connect(this->lineEdit, &QLineEdit::textChanged, this, [=]() {
		if (this->lineEdit->text().isEmpty()) {
			emit this->textEmpty();
		}
		});
}

LineEdit::~LineEdit()
{
}

bool LineEdit::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->lineEdit) {
		if (event->type() == QEvent::HoverEnter) {
			return true;
		}
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent* key_event = reinterpret_cast<QKeyEvent*> (event);
			if (key_event->key() == Qt::Key_Return) {
				if (this->lineEdit->text().isEmpty())
					return false;
				emit this->searchFriend(this->lineEdit->text());
				return true;
			}
		}
	}
	if (target == this->_close) {
		if (event->type() == QEvent::MouseButtonPress) {
			emit this->hideAddFriend();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}

void LineEdit::paintEvent(QPaintEvent*)
{
}