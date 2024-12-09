#include "../include/Ornament.h"

Ornament::Ornament(const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount, QWidget* parent)
	: FramelessWindow(parent)
{
	this->setWindowIcon(QIcon(":/Resource/ico/TablerBrandUnity.png"));

	qRegisterMetaType<QList<UserData>>("QList<UserData>");
	GLOB_UserAccount = userAccount;
	GLOB_UserName = userName;
	GLOB_UserHeadImagebytes = imagebytes;

	this->setMinimumSize(1050, 750);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(15, 0, 15, 0);
	this->setLayout(main_vbox);
	this->setAttribute(Qt::WA_DeleteOnClose);

	this->application_title_Bar = new ApplicationTitleBar(this);
	this->application_title_Bar->setUserHead(imagebytes);
	this->setTitleBar(this->application_title_Bar);

	QHBoxLayout* lay = new QHBoxLayout;
	this->application_feature_Bar = new ApplicationFeaureBar(this);

	this->stack_layout = new QStackedLayout;

	this->chat_page = new ChatPage(this);
	this->stack_layout->addWidget(this->chat_page);

	this->friend_page = new FriendPage(this);
	this->stack_layout->addWidget(this->friend_page);

	lay->addWidget(this->application_feature_Bar);
	lay->addLayout(this->stack_layout);

	main_vbox->addWidget(this->application_title_Bar);
	main_vbox->addLayout(lay);

	this->tool = new TitleTool(this);
	this->tool->setGeometry(QRect(QPoint(this->rect().right() - (this->tool->width() + 20), this->rect().top() + 70), QSize(this->tool->size())));
	this->tool->hide();

	this->systemNotification = new SystemNotification(this);
	this->systemNotification->setGeometry(QRect(QPoint(this->rect().right() + 5, this->rect().center().y() - (this->systemNotification->height() / 2)), QSize(this->systemNotification->size())));
	this->systemNotification->hide();

	connect(this->systemNotification, &SystemNotification::updateFriendList, this->friend_page, &FriendPage::IncreaseNewUserItem, Qt::DirectConnection);

	this->systemNotification_Animation = new QTimeLine(300, this);
	this->systemNotification_Animation->setEasingCurve(QEasingCurve::InOutSine);
	this->systemNotification_Animation->setUpdateInterval(0);
	this->systemNotification_Animation->setFrameRange(this->rect().right() + 5, this->rect().right() - (this->systemNotification->width() + 10));
	connect(this->systemNotification_Animation, &QTimeLine::frameChanged, this, &Ornament::SystemNotificationAnimationFrameChanged, Qt::DirectConnection);
	connect(this->systemNotification_Animation, &QTimeLine::finished, this, [=]() {
		if (this->systemNotification_Animation->direction() == QTimeLine::Backward)
			this->systemNotification->hide();
		});

	connect(this->application_title_Bar, &ApplicationTitleBar::showToolSignal, this, &Ornament::showTool, Qt::DirectConnection);
	connect(this, &Ornament::ToolStateSignal, this->application_title_Bar, &ApplicationTitleBar::setUnfoldIcon, Qt::DirectConnection);
	connect(this->tool, &TitleTool::minWindowSignal, this, &Ornament::showMinimized, Qt::DirectConnection);
	connect(this->tool, &TitleTool::maxWindowSignal, this, &Ornament::maxWindowSlot, Qt::DirectConnection);
	connect(this->tool, &TitleTool::closeWindowSignal, this, &Ornament::close, Qt::DirectConnection);
	connect(this->application_feature_Bar, &ApplicationFeaureBar::indexChanged, this->stack_layout, &QStackedLayout::setCurrentIndex, Qt::DirectConnection);
	connect(this->application_title_Bar, &ApplicationTitleBar::showAddFriendSignal, this, &Ornament::showAddFriend, Qt::DirectConnection);
	connect(this->application_title_Bar, &ApplicationTitleBar::showSystemNotification, this, &Ornament::showSystemNotification, Qt::DirectConnection);
	connect(this->friend_page, &FriendPage::createChatWindowSignal, this->chat_page, &ChatPage::DoubleClickCreateChatWindow, Qt::DirectConnection);
	connect(this->friend_page, &FriendPage::createChatWindowSignal, this, [=](UserData& user_data) {
		Q_UNUSED(user_data);
		this->stack_layout->setCurrentIndex(0);
		this->application_feature_Bar->setCurrentFeatureButton(0);
		});
	connect(this->systemNotification, &SystemNotification::NotificationEmptySignal, this, [=]() {
		this->application_title_Bar->setNotificationUnread(false);
		});

	connect(this->friend_page, &FriendPage::itemChanged, this->chat_page, &ChatPage::itemChanged, Qt::DirectConnection);

	SystemTrayIconNotification::getInstence();

	//连接服务器
	this->chat_thread = new  QThread;
	this->chat_network_manager = new ChatNetworkManager;
	this->chat_network_manager->moveToThread(this->chat_thread);
	connect(this->chat_thread, &QThread::started, this->chat_network_manager, &ChatNetworkManager::initializeSocket, Qt::DirectConnection);
	this->chat_thread->start();
	connect(this->chat_network_manager, &ChatNetworkManager::connectedSignal, this, &Ornament::startSqlThread, Qt::QueuedConnection);
	connect(this->chat_network_manager, &ChatNetworkManager::connectedSignal, this, &Ornament::startFileServiceThread, Qt::QueuedConnection);

	connect(this->chat_network_manager, &ChatNetworkManager::connecterrorSignal, this, &Ornament::deleteChatThread, Qt::QueuedConnection);
	connect(this->chat_network_manager, &ChatNetworkManager::UserLogined, this->friend_page, &FriendPage::updateFriendCurrentStatus, Qt::QueuedConnection);
	connect(this->chat_page, &ChatPage::SendUserMessage, this->chat_network_manager, &ChatNetworkManager::sendUserNormalMessage, Qt::QueuedConnection);
	connect(this->chat_network_manager, &ChatNetworkManager::acceptUserNormalMessage, this, &Ornament::dealAcceptUserNormalMessage, Qt::QueuedConnection);
	connect(this->chat_network_manager, &ChatNetworkManager::userDisconnectedSignal, this, &Ornament::dealUserDisconnected, Qt::QueuedConnection);
}

Ornament::~Ornament()
{
	this->deleteChatThread();
}

void Ornament::startSqlThread()
{
	this->application_title_Bar->setOnlineStatus(true); //设置在线状态为true
	//初始化数据库线程
	this->sql_thread = new QThread;
	this->userDataBase = new UserDatabaseManager("connect_main");
	this->userDataBase->moveToThread(this->sql_thread);
	connect(this->sql_thread, &QThread::started, this->userDataBase, &UserDatabaseManager::iniSql, Qt::DirectConnection);
	this->sql_thread->start();
	connect(this->userDataBase, &UserDatabaseManager::SearchFriendDataSignal, this, &Ornament::SearchFriendDataSignal, Qt::QueuedConnection);
	connect(this, &Ornament::searchFriendSignal, this->userDataBase, &UserDatabaseManager::selectUserDataForSearch, Qt::QueuedConnection);
	connect(this, &Ornament::SendFriendApplication, this->userDataBase, &UserDatabaseManager::increaseUserApplicationTemp, Qt::QueuedConnection);
	connect(this->userDataBase, &UserDatabaseManager::SendApplicationToServer, this->chat_network_manager, &ChatNetworkManager::sendApplication, Qt::QueuedConnection);
	connect(this->userDataBase, &UserDatabaseManager::isSendApplication, this, &Ornament::isSendApplication, Qt::QueuedConnection);
	connect(this->userDataBase, &UserDatabaseManager::existTheUserSignal, this, &Ornament::existTheUserSignal, Qt::QueuedConnection);
	connect(this->userDataBase, &UserDatabaseManager::userFriends, this->friend_page, &FriendPage::userFriendList, Qt::QueuedConnection);

	connect(this->chat_network_manager, &ChatNetworkManager::acceptUserApplication, this->userDataBase, &UserDatabaseManager::selectUserData, Qt::QueuedConnection);
	connect(this->userDataBase, &UserDatabaseManager::userDataSignal, this, &Ornament::dealUserApplication, Qt::QueuedConnection);
	connect(this->systemNotification, &SystemNotification::agreeApplicationSignal, this->userDataBase, &UserDatabaseManager::addUserFriend, Qt::QueuedConnection);
	connect(this->userDataBase, &UserDatabaseManager::AcceptedApplicationSignal, this->chat_network_manager, &ChatNetworkManager::acceptApplication, Qt::QueuedConnection);
	connect(this->chat_network_manager, &ChatNetworkManager::updateUserFriendList, this->userDataBase, &UserDatabaseManager::selectUserData, Qt::QueuedConnection);
	connect(this->userDataBase, &UserDatabaseManager::updateFriendListDataSignal, this->friend_page, &FriendPage::IncreaseNewUserItem, Qt::QueuedConnection);
}

void Ornament::startFileServiceThread()
{
	this->filework_thread = new QThread;
	this->file_work = new FileWork;
	this->file_work->moveToThread(this->filework_thread);
	connect(this->filework_thread, &QThread::started, this->file_work, &FileWork::initializeFileSocket, Qt::DirectConnection);
	this->filework_thread->start();
	connect(this->chat_page, &ChatPage::SendUserMessageForUserFileSignal, this->file_work, &FileWork::SendFileInfo, Qt::QueuedConnection);
	connect(this->file_work, &FileWork::updateUploadFileProgressSignal, this, &Ornament::updateUploadingFileProgress, Qt::QueuedConnection);
	connect(this->file_work, &FileWork::ReceiveFileSignal, this, &Ornament::dealReceiveFileForServer, Qt::QueuedConnection);
	connect(this->file_work, &FileWork::updateDownloadFileProgressSignal, this, &Ornament::updateDownloadFileProgress, Qt::QueuedConnection);
}

void Ornament::deleteChatThread()
{
	if (this->chat_thread != Q_NULLPTR) {
		this->chat_network_manager->deleteLater();
		this->chat_network_manager = Q_NULLPTR;

		this->chat_thread->exit();
		this->chat_thread->wait();
		this->chat_thread->deleteLater();
		this->chat_thread = Q_NULLPTR;

		if (this->sql_thread != Q_NULLPTR) {
			this->userDataBase->deleteLater();
			this->userDataBase = Q_NULLPTR;

			this->sql_thread->exit();
			this->sql_thread->wait();
			this->sql_thread->deleteLater();
			this->sql_thread = Q_NULLPTR;
		}

		if (this->filework_thread) {
			this->file_work->deleteLater();
			this->file_work = Q_NULLPTR;

			this->filework_thread->requestInterruption();
			this->filework_thread->exit(0);
			this->filework_thread->wait();
			this->filework_thread->deleteLater();
			this->filework_thread = Q_NULLPTR;
		}
	}
}

void Ornament::resizeEvent(QResizeEvent* event)
{
	if (this->isShowMax)
		return;
	this->tool->setGeometry(QRect(QPoint(this->rect().right() - (this->tool->width() + 20), this->rect().top() + 70), QSize(this->tool->size())));
	if (this->systemNotification->isHidden())
		this->systemNotification->setGeometry(QRect(QPoint(this->rect().right() + 5, this->rect().center().y() - (this->systemNotification->height() / 2)), QSize(this->systemNotification->size())));
	else
		this->systemNotification->setGeometry(QRect(QPoint(this->rect().right() - (this->systemNotification->width() + 10), this->rect().center().y() - (this->systemNotification->height() / 2)), QSize(this->systemNotification->size())));
	this->systemNotification_Animation->setFrameRange(this->rect().right() + 5, this->rect().right() - (this->systemNotification->width() + 10));
}

bool Ornament::eventFilter(QObject* target, QEvent* event)
{
	return false;
}

void Ornament::showTool()
{
	this->tool->isHidden() ? this->tool->show() : this->tool->hide();
	this->tool->raise();
	if (this->tool->isHidden())
		emit this->ToolStateSignal(false);
	else
		emit this->ToolStateSignal(true);
}

void Ornament::showAddFriend()
{
	this->addFriend = new AddFriend();

	connect(this->addFriend, &AddFriend::searchFriendSignal, this, &Ornament::searchFriendSignal, Qt::DirectConnection);
	connect(this, &Ornament::SearchFriendDataSignal, this->addFriend, &AddFriend::increaseSearchMember, Qt::DirectConnection);
	connect(this->addFriend, &AddFriend::sendFriendApplication, this, &Ornament::SendFriendApplication, Qt::DirectConnection);
	connect(this, &Ornament::isSendApplication, this->addFriend, &AddFriend::sendApplicationSucceedAnimation, Qt::DirectConnection);
	connect(this, &Ornament::existTheUserSignal, this->addFriend, &AddFriend::ExistTheUser, Qt::DirectConnection);
	this->addFriend->exec();
}

void Ornament::showSystemNotification()
{
	this->systemNotification->isHidden() ? this->systemNotification->show() : this->systemNotification->hide();
	this->systemNotification->raise();
	this->systemNotification_Animation->setDirection(QTimeLine::Forward);
	this->systemNotification_Animation->stop();
	this->systemNotification_Animation->start();
}

void Ornament::SystemNotificationAnimationFrameChanged(int frame)
{
	this->systemNotification->move(frame, this->systemNotification->geometry().y());
}

void Ornament::maxWindowSlot()
{
	this->isMaximized() ? this->showNormal() : this->showMaximized();
}

void Ornament::dealUserApplication(const UserData& user_data)
{
	this->application_title_Bar->setNotificationUnread(true);
	this->systemNotification->IncreaseUserApplicationItem(user_data);
}

void Ornament::dealAcceptUserNormalMessage(const QString& senderUserAccount, const QString& message)
{
	UserData user_data = this->friend_page->getUserData(senderUserAccount);
	user_data.userMessage = message;
	user_data.alignment = Qt::AlignLeft;
	user_data.messageType = ChatMessageType::TEXT;
	this->chat_page->CreateChatWindow(user_data);
	QPixmap pixmap = user_data.userHead;
	SystemTrayIconNotification::getInstence()->showMessage(user_data.userName, message, pixmap);
}

void Ornament::dealUserDisconnected(const QString& userAccount)
{
	this->friend_page->setUserDataForDisconnected(userAccount);
}

void Ornament::dealReceiveFileForServer(const QString senderAccount, const QString fileName, const qint64 fileSize)
{
	UserData user_data = this->friend_page->getUserData(senderAccount);
	QString suffix = fileName.right(3);
	if (suffix == "exe")
		user_data.fileInfo.FileType = FILETYPE::EXE;

	if (suffix == "mp3")
		user_data.fileInfo.FileType = FILETYPE::MUSIC;

	user_data.fileInfo.fileName = fileName;
	user_data.fileInfo.fileSize = QString::number(fileSize * 1.0 * OneByteForMB, 'f', 2) + "MB";
	user_data.alignment = Qt::AlignLeft;
	user_data.messageType = ChatMessageType::USERFILE;
	user_data.fileInfo.isDownloading = true;
	this->chat_page->CreateChatWindow(user_data);
	QString message = QString("[文件]") + fileName;
	QPixmap pixmap = user_data.userHead;
	SystemTrayIconNotification::getInstence()->showMessage(user_data.userName, message, pixmap);
}

void Ornament::updateUploadingFileProgress(const qreal& pos)
{
	this->chat_page->setUploadFileItemProgress(pos);
}

void Ornament::updateDownloadFileProgress(const qreal& pos)
{
	this->chat_page->updateDownloadFileProgress(pos);
}

void Ornament::mousePressEvent(QMouseEvent* event)
{
	if (this->systemNotification_Animation->state() == QTimeLine::Running)
		return;
	if (!this->systemNotification->isHidden()) {
		this->systemNotification_Animation->setDirection(QTimeLine::Backward);
		//this->systemNotification_Animation->stop();
		this->systemNotification_Animation->start();
	}

	QWidget::mousePressEvent(event);
}