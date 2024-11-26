#include "../include/Login.h"

Login::Login(QWidget* parent)
	: FramelessWindow(parent)
{
	GLOB_ScaleDpi = this->devicePixelRatioF();

	this->setFixedSize(320, 450);
	this->setMouseTracking(true);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(11, 0, 11, 11);
	this->setLayout(main_vbox);

	this->login_title_Bar = new NormalTitleBar("", this);
	this->setTitleBar(this->login_title_Bar);

	this->userHead = new QLabel(this);
	this->userHead->setFixedSize(80, 80);
	this->userHead->setScaledContents(true);
	this->userHeadPixmap.load(":/Resource/ico/TablerBrandUnity.png");
	this->userHeadPixmap = this->userHeadPixmap.scaled(QSize(this->userHead->size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->userHead->setPixmap(this->userHeadPixmap);

	this->userAccountEdit = new LineEditComponent("登录", 0, this);
	this->userPasswordEdit = new LineEditComponent("登录", 1, this);

	this->loginButton = new ButtonComponent("登录", this);
	this->loginButton->installEventFilter(this);

	QPalette pale;
	pale.setColor(QPalette::WindowText, QColor::fromString("#2d77e5"));
	this->registerButton = new QLabel("注册账号", this);
	this->registerButton->setPalette(pale);
	this->registerButton->adjustSize();
	this->registerButton->setCursor(Qt::PointingHandCursor);
	this->registerButton->installEventFilter(this);

	QVBoxLayout* vbox_1 = new QVBoxLayout;
	vbox_1->setContentsMargins(20, 11, 20, 11);
	vbox_1->addWidget(this->userAccountEdit);
	vbox_1->addSpacing(5);
	vbox_1->addWidget(this->userPasswordEdit);
	vbox_1->addSpacing(30);
	vbox_1->addWidget(this->loginButton);
	vbox_1->addSpacing(20);
	vbox_1->addWidget(this->registerButton, 0, Qt::AlignCenter);

	main_vbox->addWidget(this->login_title_Bar);
	main_vbox->addSpacing(20);
	main_vbox->addWidget(this->userHead, 0, Qt::AlignCenter);
	main_vbox->addSpacing(45);
	main_vbox->addLayout(vbox_1);
	main_vbox->addStretch();

	this->login_button_animation = new QPropertyAnimation(this->loginButton, "geometry", this);
	this->login_button_animation->setDuration(200);
	this->login_button_animation->setEasingCurve(QEasingCurve::InOutQuad);

	connect(this->loginButton, &ButtonComponent::showed, this, [=]() {
		this->login_button_animation->setStartValue(this->loginButton->geometry());
		this->login_button_animation->setEndValue(QRect(QPoint(this->loginButton->pos().x() + ((this->loginButton->width() - 200) / 2), this->loginButton->pos().y() + ((this->loginButton->height() - 20) / 2)), QSize(200, 20)));
		});

	qDebug() << "主线程" << QThread::currentThreadId();
	//初始化 数据库线程
	this->sql_thread = new QThread;
	this->userDatabase = new UserDatabaseManager("connec_login");
	this->userDatabase->moveToThread(this->sql_thread);
	this->sql_thread->start();

	connect(this->sql_thread, &QThread::started, this->userDatabase, &UserDatabaseManager::iniSql, Qt::DirectConnection);
	//	connect(this->login_title_Bar, &NormalTitleBar::closeWindowSignal, this->userDatabase, &UserDatabaseManager::closeDatabase, Qt::QueuedConnection);
	connect(this->login_title_Bar, &NormalTitleBar::closeWindowSignal, this, &Login::close, Qt::QueuedConnection);
	//connect(this->userDatabase, &UserDatabaseManager::closedDatabaseSignal, this, &Login::close, Qt::QueuedConnection);
	connect(this->userAccountEdit, &LineEditComponent::userAccountChanged, this->userDatabase, &UserDatabaseManager::selectUserHeadData, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::userHeadByteArray, this, &Login::getUserHeadBytes, Qt::QueuedConnection);
	connect(this, &Login::startloginAccountSignal, this->userDatabase, &UserDatabaseManager::VerifyUserAcocunt, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::VerifySucceed, this, &Login::VerifySucceed, Qt::QueuedConnection);
	connect(this->userDatabase, &UserDatabaseManager::VerifyFailed, this, [=]() {
		this->isLogining = false;
		qDebug() << "账号或密码错误";
		}, Qt::QueuedConnection); ;
	connect(this->userDatabase, &UserDatabaseManager::UnValidUserAccount, this, [=](bool enable) {
		this->isLogining = false;
		qDebug() << "账号不存在";
		});
}
Login::~Login()
{
}

void Login::paintEvent(QPaintEvent*)
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

void Login::getUserHeadBytes(const QPixmap& userhead)
{
	//this->userHeadPixmap = this->setPixmapDpi(this->userHead->size(), userhead);
	this->userHead->setPixmap(userhead);
}

QPixmap Login::setPixmapDpi(const QSize& size, const QPixmap& pixmap)
{
	QPixmap resultPixmap = pixmap.scaled(size * GLOB_ScaleDpi, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	return resultPixmap;
}

bool Login::eventFilter(QObject* target, QEvent* event)
{
	if (target == this->registerButton) {
		if (event->type() == QEvent::MouseButtonPress) {
			if (this->registerUserAccount != Q_NULLPTR) {
				this->registerUserAccount->raise();
				return true;
			}
			this->registerUserAccount = new RegisterUserAccount(Q_NULLPTR);
			this->registerUserAccount->show();
			connect(this->registerUserAccount, &RegisterUserAccount::closeWindowSignal, this, &Login::deleteRegisterWindow, Qt::DirectConnection);
			return true;
		}
	}
	if (target == this->loginButton) {
		if (event->type() == QEvent::MouseButtonPress) {
			this->login_button_animation->setDirection(QPropertyAnimation::Forward);
			this->login_button_animation->stop();
			this->login_button_animation->start();
			if (this->isLogining || this->userAccountEdit->currentText().isEmpty() || this->userPasswordEdit->currentText().isEmpty())
				return false;
			emit this->startloginAccountSignal(this->userAccountEdit->currentText(), this->userPasswordEdit->currentText());
			this->isLogining = true;
			return true;
		}
		if (event->type() == QEvent::MouseButtonRelease) {
			this->login_button_animation->setDirection(QPropertyAnimation::Backward);
			this->login_button_animation->stop();
			this->login_button_animation->start();
			return true;
		}
	}
	return QWidget::eventFilter(target, event);
}

void Login::deleteRegisterWindow()
{
	this->registerUserAccount->disconnect(this);
	delete this->registerUserAccount;
	this->registerUserAccount = Q_NULLPTR;
}

void Login::closeEvent(QCloseEvent*)
{
	if (this->registerUserAccount)
		this->deleteRegisterWindow();
	this->deleteSqlThread();
}

void Login::deleteSqlThread()
{
	if (this->userDatabase) {
		this->userDatabase->deleteLater();
		this->userDatabase = Q_NULLPTR;
		this->sql_thread->exit();
		this->sql_thread->wait();
		this->sql_thread = Q_NULLPTR;
	}
}