#ifndef RFIDINTERFACE_H
#define RFIDINTERFACE_H

#include <QObject>
#include <QThread>

class NFCWorker : public QObject
  {
      Q_OBJECT

  public slots:
      void searchForTags();

  signals:
      void tagFound(const QString &tagId);
  };


class RFIDInterface : public QObject
{
	Q_OBJECT
public:
	explicit RFIDInterface(QObject *parent = nullptr);
    ~RFIDInterface();

private:
    QThread m_nfcThread;

public slots:
    void handleTag(const QString &tagId);

signals:
    void operate();
    void tagRecognized(const QString &tagId);
};

#endif // RFIDINTERFACE_H
