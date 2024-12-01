#include "../include/ChatWindow.h"

ChatWindow::ChatWindow(const UserData& user_data, QWidget* parent)
	: QWidget(parent)
{
	this->m_userData = user_data;
	this->currentChatWindowIndex = user_data.index;
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	//main_vbox->setContentsMargins(0, 0, 0, 0);
	this->setLayout(main_vbox);

	this->chat_title = new ChatTitle(user_data.userName, user_data.userHead, this);

	QPalette pale;
	pale.setColor(QPalette::Window, Qt::transparent);

	this->chat_list = new QListWidget(this);
	this->chat_list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	this->chat_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	chat_list->setSpacing(5);
	this->chat_list->setFrameShape(QFrame::NoFrame);
	MessageDelegate* delegate = new MessageDelegate(this);
	this->chat_list->setItemDelegate(delegate);
	this->chat_list->setPalette(pale);

	this->message_edit = new ChatMessageEdit(this);

	main_vbox->addWidget(this->chat_title);
	main_vbox->addWidget(this->chat_list);
	main_vbox->addWidget(this->message_edit);

	connect(this->message_edit, &ChatMessageEdit::SendUserMessage, this, &ChatWindow::dealUserSendMessage, Qt::DirectConnection);
	connect(this->message_edit, &ChatMessageEdit::MyMessageSignal, this, &ChatWindow::IncreaseMyMessageItem, Qt::DirectConnection);
}

ChatWindow::~ChatWindow()
{
}

UserData ChatWindow::currentUserData() const
{
	return m_userData;
}

void ChatWindow::IncreaseMessageItem(const UserData& user_data)
{
	QListWidgetItem* item = new QListWidgetItem(this->chat_list);
	item->setData(Qt::UserRole, QVariant::fromValue(user_data));
	this->chat_list->addItem(item);
}

void ChatWindow::IncreaseMyMessageItem(const QString& message)
{
	UserData user_data;
	user_data.alignment = Qt::AlignRight;
	user_data.userAccount = QString::number(GLOB_UserAccount);
	user_data.userName = GLOB_UserName;
	QPixmap userHead = RoundImage::RoundImageFromByteArray(GLOB_UserHeadImagebytes);
	user_data.userHead = userHead;
	user_data.userMessage = message;
	user_data.index = this->m_userData.index;

	this->IncreaseMessageItem(user_data);
}

void ChatWindow::dealUserSendMessage(const QString& message)
{
	if (!this->m_userData.status)
	{
		qDebug() << "当前好友未在线";
		return;
	}

	emit this->SendUserMessage(QString::number(GLOB_UserAccount), this->m_userData.userAccount, message);
}

void ChatWindow::setChatWindowData(const UserData& user_data)
{
	this->m_userData = user_data;
}

ChatTitle::ChatTitle(const QString& user_name, const QPixmap& user_head, QWidget* parent) :QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	//this->shadow = new QGraphicsDropShadowEffect(this);
	//this->shadow->setOffset(0, 0);
	//this->shadow->setColor(Qt::gray);
	//this->shadow->setBlurRadius(8);
	//this->setGraphicsEffect(this->shadow);

	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(40, 40);
	this->userHead->setScaledContents(true);
	this->userHead->setPixmap(user_head.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	this->userName = new QLabel(user_name, this);
	this->userName->adjustSize();
	main_lay->addWidget(this->userHead);
	main_lay->addWidget(this->userName);
	main_lay->addStretch();
}

ChatTitle::~ChatTitle()
{
}

void ChatTitle::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 10, 10);
}

ChatMessageEdit::ChatMessageEdit(QWidget* parent) :QWidget(parent)
{
	this->setFixedHeight(130);
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);
	main_vbox->setContentsMargins(0, 0, 0, 0);

	this->message_edit = new QTextEdit(this);
	this->message_edit->setFrameShape(QFrame::NoFrame);
	this->message_edit->setPlaceholderText("Enter Something...");
	this->message_edit->installEventFilter(this);

	this->send_button = new SendMessageButton(this);
	main_vbox->addSpacing(5);
	main_vbox->addWidget(this->message_edit);
	main_vbox->addWidget(this->send_button, 0, Qt::AlignRight);
}

void ChatMessageEdit::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(QColor(0, 0, 0, 50));
	painter.setBrush(Qt::NoBrush);
	painter.drawLine(QPoint(this->rect().topLeft()), QPoint(this->rect().topRight()));
	QWidget::paintEvent(event);
}

bool ChatMessageEdit::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->message_edit) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent* key = reinterpret_cast<QKeyEvent*>(event);
			if (key->key() == Qt::Key_Return) {
				emit this->SendUserMessage(this->message_edit->toPlainText());
				emit this->MyMessageSignal(this->message_edit->toPlainText());
				this->message_edit->clear();
				return true;
			}
		}
	}
	return QWidget::eventFilter(target, event);
}

SendMessageButton::SendMessageButton(QWidget* parent) :QWidget(parent)
{
	this->setFixedSize(60, 30);
	this->setCursor(Qt::PointingHandCursor);
}

void SendMessageButton::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor::fromString("#6a4cff"));
	painter.drawRoundedRect(this->rect(), 10, 10);
	painter.restore();

	QFont font;
	font.setBold(true);
	painter.save();
	painter.setPen(Qt::white);
	painter.setFont(font);
	painter.setBrush(Qt::NoBrush);
	painter.drawText(this->rect(), Qt::AlignCenter, QString("发送"));
	painter.restore();
}
