#include "../include/RegisterUserAccount.h"

RegisterUserAccount::RegisterUserAccount(QWidget* parent)
	: FramelessWindow(parent)
{
	GLOB_ScaleDpi = this->devicePixelRatioF();

	this->setFixedSize(320, 450);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(11, 0, 11, 11);
	this->setLayout(main_vbox);

	this->titleBar = new NormalTitleBar("", this);
	this->setTitleBar(this->titleBar);

	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(80, 80);
	this->userHead->setScaledContents(true);
	this->userHeadPixmap.load(":/Resource/ico/TablerBrandUnity.png");
	this->userHeadPixmap = this->userHeadPixmap.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->userHead->setPixmap(this->userHeadPixmap);
	this->userHead->setCursor(Qt::PointingHandCursor);
	this->userHead->installEventFilter(this);

	this->userName = new LineEditComponent("注册", 0, this);
	this->userPassword = new LineEditComponent("注册", 1, this);

	this->registerButton = new ButtonComponent("注册", this);
	this->registerButton->installEventFilter(this);

	QVBoxLayout* vbox_1 = new QVBoxLayout;
	vbox_1->setContentsMargins(20, 11, 20, 11);
	vbox_1->addWidget(this->userName);
	vbox_1->addSpacing(5);
	vbox_1->addWidget(this->userPassword);
	vbox_1->addSpacing(30);
	vbox_1->addWidget(this->registerButton);
	vbox_1->addSpacing(20);

	main_vbox->addWidget(this->titleBar);
	main_vbox->addSpacing(20);
	main_vbox->addWidget(this->userHead, 0, Qt::AlignCenter);
	main_vbox->addSpacing(45);
	main_vbox->addLayout(vbox_1);
	main_vbox->addStretch();

	this->notification = new SystemNotification(this);
	this->notification->setGeometry(QRect(QPoint(this->rect().center().x() - (this->notification->width() / 2), this->rect().top() - (this->notification->height())), QSize(this->notification->size())));
	this->notification->hide();

	this->animation = new QTimeLine(500, this);
	this->animation->setUpdateInterval(0);
	this->animation->setFrameRange(this->rect().top() - (this->notification->height()), 20);

	connect(this->animation, &QTimeLine::frameChanged, this, [=](int frame) {
		this->notification->move(this->notification->geometry().x(), frame);
		});
	qDebug() << "主线程" << QThread::currentThreadId();
	//初始化 数据库线程
	this->sql_thread = new QThread;
	this->userDatabase = new UserDatabaseManager("connect_register");
	this->userDatabase->moveToThread(this->sql_thread);
	this->sql_thread->start();
	connect(this->sql_thread, &QThread::started, this->userDatabase, &UserDatabaseManager::iniSql, Qt::DirectConnection);
	//connect(this->titleBar, &NormalTitleBar::closeWindowSignal, this->userDatabase, &UserDatabaseManager::closeDatabase, Qt::QueuedConnection);
	connect(this->titleBar, &NormalTitleBar::closeWindowSignal, this, &RegisterUserAccount::close, Qt::QueuedConnection);
	//connect(this->userDatabase, &UserDatabaseManager::closedDatabaseSignal, this, &RegisterUserAccount::closeWindowSignal, Qt::QueuedConnection);
	connect(this->userName, &LineEditComponent::userAccountChanged, this->userDatabase, &UserDatabaseManager::selectUserHeadData, Qt::QueuedConnection);
	connect(this, &RegisterUserAccount::registerUserAccount, this->userDatabase, &UserDatabaseManager::RegisterUserAccount, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::RegisterSucceedSignal, this, [=]() {
		this->animation->setDirection(QTimeLine::Forward);
		this->notification->setText("注册成功");
		this->notification->setGeometry(QRect(QPoint(this->rect().center().x() - (this->notification->width() / 2), this->rect().top() - (this->notification->height())), QSize(this->notification->size())));
		this->notification->show();
		this->animation->start();
		QTimer::singleShot(1000, this, [=]() {
			this->animation->setDirection(QTimeLine::Backward);
			this->animation->start();
			});
		});
}

RegisterUserAccount::~RegisterUserAccount()
{
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

bool RegisterUserAccount::eventFilter(QObject* target, QEvent* event)
{
	if (this->userHead == target) {
		if (event->type() == QEvent::MouseButtonPress) {
			QString filepath = QFileDialog::getOpenFileName(this, "选择头像", "/");
			if (filepath.isEmpty())
				return false;
			this->imagebytes.clear();
			QBuffer buffer(&this->imagebytes);
			buffer.open(QIODevice::WriteOnly);
			QPixmap pixmap;
			pixmap.load(filepath);
			pixmap.save(&buffer, "JPG");
			buffer.close();
			this->userHeadPixmap = RoundImage::RoundImageFromStringPath(filepath);
			this->userHead->setPixmap(userHeadPixmap);
			return true;
		}
	}
	if (this->registerButton == target) {
		if (event->type() == QEvent::MouseButtonPress) {
			if (this->userName->currentText().isEmpty() || this->userPassword->currentText().isEmpty())
				return false;
			emit this->registerUserAccount(this->imagebytes, this->userName->currentText(), this->userPassword->currentText());
			return true;
		}
	}
	return FramelessWindow::eventFilter(target, event);
}

void RegisterUserAccount::closeEvent(QCloseEvent*)
{
	if (this->sql_thread) {
		this->userDatabase->deleteLater();
		this->userDatabase = Q_NULLPTR;

		this->sql_thread->exit(0);
		this->sql_thread->wait();
		this->sql_thread->deleteLater();
		this->sql_thread = Q_NULLPTR;
	}
}