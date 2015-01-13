#ifndef VLCPLAYWINDOW_H
#define VLCPLAYWINDOW_H

#include "vlcplaywindow_global.h"
#include <QString>
#include <QWidget>
#include <QFrame>
#include <QMouseEvent>
#include <QLabel>
#include "Enums.h"

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_manager_t;
struct libvlc_event_t;
struct libvlc_media_t;

namespace Ui {
	class PlayWindow;
}

class VLCPLAYWINDOWSHARED_EXPORT VlcPlayWindow : public QWidget
{
	Q_OBJECT

public:
	explicit VlcPlayWindow(QWidget *parent = 0);
	~VlcPlayWindow();
	void setRtspUrl(QString rtspUrl);
	bool startPlay();
	bool stopPlay();
	bool pause();
	bool resume();
	void setWindowIndex(int winIdx);
	QString record(const QString &name,
		const QString &path,
		const Vlc::Mux &mux,
		bool duplicate);
	QString record(const QString &name,
		const QString &path,
		const Vlc::Mux &mux,
		const Vlc::AudioCodec &audioCodec,
		const Vlc::VideoCodec &videoCodec,
		bool duplicate);
	QString record(const QString &name,
		const QString &path,
		const Vlc::Mux &mux,
		const Vlc::AudioCodec &audioCodec,
		const Vlc::VideoCodec &videoCodec,
		int bitrate,
		int fps,
		int scale,
		bool duplicate);
	void setProgram(int program);
	void setOption(const QString &option);
	void setOptions(const QStringList &options);
protected:
	//bool event(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *);
	bool eventFilter(QObject *obj, QEvent *ev);
private:
	Ui::PlayWindow *ui;
	QFrame *m_playFrame;
	QFrame *m_showFrame;
	QLabel *m_showLabel;
	WId m_playId;
	QString m_rtspUrl;
	libvlc_instance_t *m_vlcObject;
	libvlc_media_player_t *m_vlcPlayer;
	libvlc_event_manager_t *_vlcEvents;
	libvlc_media_t * _vlcMedia;

	int m_currWinIdx;
	bool initVlc();
	void createCoreConnections();
	void removeCoreConnections();
	static void libvlc_callback(const libvlc_event_t *event,void *data);
public slots:
	void updateInterface();
signals:
	void chooseWindow(int currWinIdx);
signals:
	/*!
	\brief Signal sent on backward
	*/
	void backward();

	/*!
	\brief Signal sent on buffering
	\param float buffer
	*/
	void buffering(float);

	/*!
	\brief Signal sent when end reached
	*/
	void end();

	/*!
	\brief Signal sent on error
	*/
	void error();

	/*!
	\brief Signal sent on forward
	*/
	void forward();

	/*!
	\brief Signal sent on length change
	\param int length
	*/
	void lengthChanged(int);

	/*!
	\brief Signal sent on media change
	\param libvlc_media_t * media
	*/
	void mediaChanged(libvlc_media_t *);

	/*!
	\brief Signal sent nothing speciall happened
	*/
	void nothingSpecial();

	/*!
	\brief Signal sent when opening
	*/
	void opening();

	/*!
	\brief Signal sent on pausable change
	\param bool pausable
	*/
	void pausableChanged(bool);

	/*!
	\brief Signal sent when paused
	*/
	void paused();

	/*!
	\brief Signal sent when playing
	*/
	void playing();

	/*!
	\brief Signal sent on position change
	\param float position
	*/
	void positionChanged(float);

	/*!
	\brief Signal sent on seekable change
	\param bool seekable
	*/
	void seekableChanged(bool);

	/*!
	\brief Signal sent on snapshot taken
	\param QString filename
	*/
	void snapshotTaken(const QString &);

	/*!
	\brief Signal sent when stopped
	*/
	void stopped();

	/*!
	\brief Signal sent on time change
	\param int time
	*/
	void timeChanged(int);

	/*!
	\brief Signal sent on title change
	\param int title
	*/
	void titleChanged(int);

	/*!
	\brief Signal sent when video output is available
	\param int vout count
	*/
	void vout(int);

	/*!
	\brief Signal sent when state of the player changed
	*/
	void stateChanged();
};

#endif // VLCPLAYWINDOW_H
