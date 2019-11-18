#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include <QObject>
#include <QThread>

class GPIOWorker : public QThread
  {
      Q_OBJECT

  public slots:
      void checkButtonStatus();

  signals:
      void buttonPressed(int buttonId);
  };

class GPIOInterface : public QObject
{
	Q_OBJECT
public:
	explicit GPIOInterface(QObject *parent = nullptr);
    ~GPIOInterface();

private:
    QThread m_gpioThread;
signals:
	void nextPressed();

public slots:
    void handleButtonStatus(int buttonId);

signals:
    void operate();
    void playButtonPressed();
    void pauseButtonPressed();
    void nextButtonPressed();
    void prevBurronPressed();
};

#endif
