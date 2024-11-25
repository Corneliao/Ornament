#include "../include/RegisterUserAccount.h"

RegisterUserAccount::RegisterUserAccount(QWidget* parent)
	: FramelessWindow(parent)
{
	GLOB_ScaleDpi = this->devicePixelRatioF();

	this->setFixedSize(320, 450);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(11, 0, 11, 11);
	this->setLayout(main_vbox);

	this->login_title_Bar = new LoginTitleBar(this);
	this->setTitleBar(this->login_title_Bar);

	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(80, 80);
	this->userHead->setScaledContents(true);
	this->userHeadPixmap.load(":/Resource/ico/TablerBrandUnity.png");
	this->userHeadPixmap = this->userHeadPixmap.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->userHead->setPixmap(this->userHeadPixmap);

	this->userAccountEdit = new LineEditComponent("注册", 0, this);
	this->userPasswordEdit = new LineEditComponent("注册", 1, this);

	this->loginButton = new ButtonComponent("注册", this);

	QVBoxLayout* vbox_1 = new QVBoxLayout;
	vbox_1->setContentsMargins(20, 11, 20, 11);
	vbox_1->addWidget(this->userAccountEdit);
	vbox_1->addSpacing(5);
	vbox_1->addWidget(this->userPasswordEdit);
	vbox_1->addSpacing(30);
	vbox_1->addWidget(this->loginButton);
	vbox_1->addSpacing(20);

	main_vbox->addWidget(this->login_title_Bar);
	main_vbox->addSpacing(20);
	main_vbox->addWidget(this->userHead, 0, Qt::AlignCenter);
	main_vbox->addSpacing(45);
	main_vbox->addLayout(vbox_1);
	main_vbox->addStretch();

	qDebug() << "主线程" << QThread::currentThreadId();
	//初始化 数据库线程
	this->sql_thread = new QThread;
	this->userDatabase = new UserDatabaseManager("connect_register");
	this->userDatabase->moveToThread(this->sql_thread);
	this->sql_thread->start();
	connect(this->sql_thread, &QThread::started, this->userDatabase, &UserDatabaseManager::iniSql, Qt::DirectConnection);
	connect(this->login_title_Bar, &LoginTitleBar::closeWindowSignal, this->userDatabase, &UserDatabaseManager::closeDatabase, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::closedDatabaseSignal, this, &RegisterUserAccount::closeWindowSignal, Qt::QueuedConnection);
	connect(this->userAccountEdit, &LineEditComponent::userAccountChanged, this->userDatabase, &UserDatabaseManager::selectUserData, Qt::QueuedConnection);
}

RegisterUserAccount::~RegisterUserAccount()
{
	if (this->sql_thread) {
		qDebug() << "释放线程";
		this->sql_thread->exit(0);
		this->sql_thread->wait();
		this->sql_thread->deleteLater();
		this->sql_thread = Q_NULLPTR;
		this->userDatabase->deleteLater();
		this->userDatabase = Q_NULLPTR;
	}
}

void RegisterUserAccount::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	QLinearGradient gradient(QPoint(this->rect().topLeft()), QPoint(this->rect().bottomLeft()));
	gradient.setColorAt(0.2, QColor::fromString("#ccf4fe"));
	gradient.setColorAt(0.5, Qt::white);
	painter.setBrush(gradient);
	painter.drawRect(this->rect());
}