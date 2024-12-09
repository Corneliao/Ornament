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
	this->chat_list->setStyleSheet("QListWidget::item:hover{background:transparent};");
	chat_list->setSpacing(5);
	this->chat_list->setFrameShape(QFrame::NoFrame);
	this->chat_list->setPalette(pale);

	this->message_edit = new ChatMessageEdit(this);

	main_vbox->addWidget(this->chat_title);
	main_vbox->addWidget(this->chat_list);
	main_vbox->addWidget(this->message_edit);

	connect(this->message_edit, &ChatMessageEdit::SendUserMessage, this, &ChatWindow::dealUserSendMessage, Qt::DirectConnection);
	connect(this->message_edit, &ChatMessageEdit::MyMessageForFileSignal, this, &ChatWindow::IncreaseMessageItemForEXE, Qt::DirectConnection);
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
	item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
	MessageItemWidget* itemWidget = new MessageItemWidget(user_data, this);
	item->setSizeHint(itemWidget->size());
	item->setData(Qt::UserRole, QVariant::fromValue(user_data));
	this->chat_list->addItem(item);
	this->chat_list->setItemWidget(item, itemWidget);
	this->chat_list->scrollToBottom();
}

void ChatWindow::dealUserSendMessage(const QString& message)
{
	UserData user_data;
	user_data.alignment = Qt::AlignRight;
	user_data.userAccount = QString::number(GLOB_UserAccount);
	user_data.userName = GLOB_UserName;
	QPixmap userHead = RoundImage::RoundImageFromByteArray(GLOB_UserHeadImagebytes);
	user_data.userHead = userHead;
	user_data.userMessage = message;
	user_data.messageType = ChatMessageType::TEXT;
	user_data.index = this->m_userData.index;
	user_data.receiverUserAccount = this->m_userData.userAccount;

	this->IncreaseMessageItem(user_data);
	emit this->modifyChatListItemData(user_data);

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

void ChatWindow::IncreaseMessageItemForEXE(const FileInfoData& file_data)
{
	UserData user_data;
	user_data.alignment = Qt::AlignRight;
	user_data.userAccount = QString::number(GLOB_UserAccount);
	user_data.userName = GLOB_UserName;
	QPixmap userHead = RoundImage::RoundImageFromByteArray(GLOB_UserHeadImagebytes);
	user_data.userHead = userHead;
	user_data.fileInfo = file_data;
	user_data.fileInfo.fileSize = QString::number(file_data.fileSize.toLongLong() * 1.0 * OneByteForMB, 'f', 2) + "MB";
	user_data.fileInfo.isUploading = true;
	user_data.messageType = ChatMessageType::USERFILE;
	user_data.index = this->m_userData.index;
	user_data.receiverUserAccount = this->m_userData.userAccount;

	this->IncreaseMessageItem(user_data);
	emit this->modifyChatListItemData(user_data);
	if (!this->m_userData.status)
		return;
	ReceiverFileUserAccountTemp = this->m_userData.userAccount;
	emit this->SendUserMessageForUserFileSignal(QString::number(GLOB_UserAccount), this->m_userData.userAccount, file_data);
}

void ChatWindow::setUploadFileItemProgress(const qreal& pos)
{
	if (this->m_userData.userAccount != ReceiverFileUserAccountTemp)
		return;
	for (int i = 0; i < this->chat_list->count(); i++) {
		QListWidgetItem* item = this->chat_list->item(i);
		if (item) {
			MessageItemWidget* itemWidget = qobject_cast<MessageItemWidget*>(this->chat_list->itemWidget(item));
			if (itemWidget && itemWidget->currentMessageItemData().fileInfo.isUploading) {
				itemWidget->setSliderPosition(pos);
				return;
			}
		}
	}
}

void ChatWindow::updateDownloadFileItemProgress(const qreal& pos)
{
	if (this->m_userData.userAccount != SenderFileUserAccountTemp)
		return;
	for (int i = 0; i < this->chat_list->count(); i++) {
		QListWidgetItem* item = this->chat_list->item(i);
		if (item) {
			MessageItemWidget* itemWidget = qobject_cast<MessageItemWidget*>(this->chat_list->itemWidget(item));
			if (itemWidget && itemWidget->currentMessageItemData().fileInfo.isDownloading) {
				itemWidget->setSliderPosition(pos);
				return;
			}
		}
	}
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

	QHBoxLayout* lay = new QHBoxLayout;

	this->file_button = new QLabel(this);

	this->file_button->setFixedSize(23, 23);
	this->file_button->setScaledContents(true);
	QPixmap pixmap(":/Resource/ico/file_unsel.png");
	this->file_button->setPixmap(pixmap);
	this->file_button->setCursor(Qt::PointingHandCursor);
	this->file_button->setAttribute(Qt::WA_Hover);
	this->file_button->installEventFilter(this);

	this->emoji_button = new QLabel(this);
	this->emoji_button->setFixedSize(25, 25);
	this->emoji_button->setScaledContents(true);
	pixmap.load(":/Resource/ico/emoji_unsel.png");
	this->emoji_button->setPixmap(pixmap);
	this->emoji_button->setCursor(Qt::PointingHandCursor);
	this->emoji_button->setAttribute(Qt::WA_Hover);
	this->emoji_button->installEventFilter(this);

	this->send_button = new SendMessageButton(this);

	lay->setSpacing(15);
	lay->addWidget(this->file_button);
	lay->addWidget(this->emoji_button);
	lay->addStretch();
	lay->addWidget(this->send_button);
	main_vbox->addSpacing(5);
	main_vbox->addWidget(this->message_edit);
	main_vbox->addLayout(lay);
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
	QPixmap pixmap;
	if (target == this->message_edit) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent* key = reinterpret_cast<QKeyEvent*>(event);
			if (key->key() == Qt::Key_Return) {
				if (this->message_edit->toPlainText().isEmpty())
					return true;
				emit this->SendUserMessage(this->message_edit->toPlainText());
				this->message_edit->clear();
				return true;
			}
		}
	}
	if (target == this->file_button) {
		if (event->type() == QEvent::HoverEnter) {
			pixmap.load(":/Resource/ico/file_sel.png");
			this->file_button->setPixmap(pixmap);
			return true;
		}
		if (event->type() == QEvent::HoverLeave) {
			pixmap.load(":/Resource/ico/file_unsel.png");
			this->file_button->setPixmap(pixmap);
			return true;
		}
		if (event->type() == QEvent::MouseButtonPress) {
			QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "/");
			if (!filePath.isEmpty()) {
				FileInfoData data;
				data.filePath = filePath;
				QFileInfo info(filePath);
				if (info.suffix() == "exe")
					data.FileType = FILETYPE::EXE;
				else if (info.suffix() == "mp3")
					data.FileType = FILETYPE::MUSIC;

				QString fileName = info.fileName();
				data.fileName = fileName;
				data.fileSize = QString::number(info.size());
				emit this->MyMessageForFileSignal(data);
			}

			return true;
		}
	}
	if (target == this->emoji_button) {
		if (event->type() == QEvent::HoverEnter) {
			pixmap.load(":/Resource/ico/emoji_sel.png");
			this->emoji_button->setPixmap(pixmap);
			return true;
		}
		if (event->type() == QEvent::HoverLeave) {
			pixmap.load(":/Resource/ico/emoji_unsel.png");
			this->emoji_button->setPixmap(pixmap);
			return true;
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
