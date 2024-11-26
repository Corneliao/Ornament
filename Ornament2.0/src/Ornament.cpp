﻿#include "../include/Ornament.h"

Ornament::Ornament(const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount, QWidget* parent)
	: FramelessWindow(parent)
{
	GLOB_UserAccount = userAccount;
	GLOB_UserName = userName;
	//GLOB_UserHead = userhead_pixmap;

	this->setMinimumSize(1050, 750);
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(0, 0, 0, 0);
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

	lay->addLayout(this->stack_layout);
	lay->addWidget(this->application_feature_Bar);

	main_vbox->addWidget(this->application_title_Bar);
	main_vbox->addLayout(lay);

	this->tool = new TitleTool(this);
	this->tool->setGeometry(QRect(QPoint(this->rect().right() - (this->tool->width() + 20), this->rect().top() + 70), QSize(this->tool->size())));
	this->tool->hide();

	//this->addFriend = new AddFriend();
	//this->addFriend->setGeometry(QRect(this->rect().center().x() - (this->addFriend->width() / 2), this->rect().top() + 80, this->addFriend->width(), this->addFriend->height()));

	connect(this->application_title_Bar, &ApplicationTitleBar::showToolSignal, this, &Ornament::showTool, Qt::DirectConnection);
	connect(this, &Ornament::ToolStateSignal, this->application_title_Bar, &ApplicationTitleBar::setUnfoldIcon, Qt::DirectConnection);
	connect(this->tool, &TitleTool::minWindowSignal, this, &Ornament::showMinimized, Qt::DirectConnection);
	connect(this->tool, &TitleTool::maxWindowSignal, this, &Ornament::maxWindowSlot, Qt::DirectConnection);
	connect(this->tool, &TitleTool::closeWindowSignal, this, &Ornament::close, Qt::DirectConnection);
	connect(this->application_feature_Bar, &ApplicationFeaureBar::indexChanged, this->stack_layout, &QStackedLayout::setCurrentIndex, Qt::DirectConnection);
	connect(this->application_title_Bar, &ApplicationTitleBar::showAddFriendSignal, this, &Ornament::showAddFriend, Qt::DirectConnection);

	qDebug() << "主线程" << QThread::currentThreadId();
	//连接服务器
	this->chat_thread = new  QThread;
	this->chat_network_manager = new ChatNetworkManager;
	chat_network_manager->moveToThread(this->sql_thread);
	connect(this->chat_thread, &QThread::started, this->chat_network_manager, &ChatNetworkManager::initializeSocket, Qt::DirectConnection);
	this->chat_thread->start();
	connect(this->chat_network_manager, &ChatNetworkManager::connectedSignal, this, &Ornament::startSqlThread, Qt::QueuedConnection);
	connect(this->chat_network_manager, &ChatNetworkManager::connecterrorSignal, this, &Ornament::deleteChatThread, Qt::QueuedConnection);
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
	connect(this->userDataBase, &UserDatabaseManager::SendApplicationToServer, this->chat_network_manager, &ChatNetworkManager::sendApplication, Qt::DirectConnection);
}

void Ornament::deleteChatThread()
{
	if (this->chat_thread != Q_NULLPTR) {
		this->chat_network_manager->deleteLater();
		this->chat_network_manager = Q_NULLPTR;

		this->chat_thread->quit();
		this->chat_thread->wait();
		this->chat_thread->deleteLater();
		this->chat_thread = Q_NULLPTR;

		if (this->sql_thread != Q_NULLPTR) {
			this->userDataBase->deleteLater();
			this->userDataBase = Q_NULLPTR;

			this->sql_thread->quit();
			this->sql_thread->wait();
			this->sql_thread->deleteLater();
			this->sql_thread = Q_NULLPTR;
		}
	}
}

void Ornament::resizeEvent(QResizeEvent* event)
{
	if (this->isShowMax)
		return;
	this->tool->setGeometry(QRect(QPoint(this->rect().right() - (this->tool->width() + 20), this->rect().top() + 70), QSize(this->tool->size())));
}

bool Ornament::eventFilter(QObject* target, QEvent* event)
{
	return false;
}

void Ornament::showEvent(QShowEvent*)
{
	HWND hCurWnd = GetForegroundWindow();
	DWORD dwMyID = GetCurrentThreadId();
	DWORD dwCurID = GetWindowThreadProcessId(hCurWnd, NULL);
	AttachThreadInput(dwCurID, dwMyID, TRUE);
	SetForegroundWindow((HWND)winId());
	AttachThreadInput(dwCurID, dwMyID, FALSE);
	SwitchToThisWindow((HWND)winId(), TRUE);
	SetActiveWindow((HWND)winId());
	this->setFocus();
}

void Ornament::showTool()
{
	this->tool->isHidden() ? this->tool->show() : this->tool->hide();
	if (this->tool->isHidden())
		emit this->ToolStateSignal(false);
	else
		emit this->ToolStateSignal(true);
}

void Ornament::showAddFriend()
{
	this->addFriend = new AddFriend();
	this->addFriend->move(600, (screenSize.height() - this->addFriend->height()) / 2);
	connect(this->addFriend, &AddFriend::searchFriendSignal, this, &Ornament::searchFriendSignal, Qt::DirectConnection);
	connect(this, &Ornament::SearchFriendDataSignal, this->addFriend, &AddFriend::increaseSearchMember, Qt::DirectConnection);
	connect(this->addFriend, &AddFriend::sendFriendApplication, this, &Ornament::SendFriendApplication, Qt::DirectConnection);
	this->addFriend->exec();
}

void Ornament::maxWindowSlot()
{
	this->isMaximized() ? this->showNormal() : this->showMaximized();
}

void Ornament::changeEvent(QEvent* event)
{
}