#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QTimer>
#include <iostream>
#include <limits>
#include <random>
#include <string>

class Generator : public QObject {
  Q_OBJECT

public:
  Generator() {
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &Generator::Update);
    timer_->start(1000);
  }

  virtual ~Generator() { delete timer_; }

public slots:
  void Update() {
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<int> distr(std::numeric_limits<int>::min());
    emit NumberGenerated(distr(engine));
  }

signals:
  void NumberGenerated(int value);

private:
  QTimer* timer_;
};

class UI : public QMainWindow {
  Q_OBJECT

public:
  UI() {
    label_ = new QLabel(this);
    label_->setAlignment(Qt::AlignCenter);
    setCentralWidget(label_);
  }

  virtual ~UI() { delete label_; }

public slots:
  void Update(int value) {
    label_->setText(QString::fromStdString(std::to_string(value)));
  }

private:
  QLabel* label_;
};

class Sender : public QObject {
  Q_OBJECT;

public:
  Sender() {}

public slots:
  void Update(int value) { std::cout << value << std::endl; }
};

class WorkflowManager {
public:
  WorkflowManager() {
    ui_ = new UI();
    generator_ = new Generator();
    sender_ = new Sender();

    QObject::connect(generator_, &Generator::NumberGenerated, ui_, &UI::Update);
    QObject::connect(generator_, &Generator::NumberGenerated, sender_,
      &Sender::Update);

    generator_->Update();
  }

  virtual ~WorkflowManager() {
    delete sender_;
    delete generator_;
    delete ui_;
  }

  void show() { ui_->show(); }

private:
  UI* ui_;
  Generator* generator_;
  Sender* sender_;
};

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  WorkflowManager manager;
  manager.show();

  app.exec();
}

#include "main.moc"
