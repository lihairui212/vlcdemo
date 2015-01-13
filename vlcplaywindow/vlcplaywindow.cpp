#include "vlcplaywindow.h"
#include "ui_playwindow.h"
#include "vlc.h"
#include <QHBoxLayout>
#include <QTimer>
#include <QDir>

#define qtu( i ) ((i).toUtf8().constData())

VlcPlayWindow::VlcPlayWindow(QWidget *parent) :
QWidget(parent),ui(new Ui::PlayWindow)
{
	ui->setupUi(this);
	
	m_vlcPlayer = NULL;
	m_currWinIdx = 0;
	initVlc();
	
	m_playFrame = new QFrame(this);
	QHBoxLayout *hLayout = new QHBoxLayout(this);
	hLayout->setMargin(1);
	hLayout->addWidget(m_playFrame);
	setLayout(hLayout);

	m_playFrame->installEventFilter(this);
	m_playId = m_playFrame->winId();

	m_showFrame = new QFrame(this);
	QHBoxLayout *hLayout_frame = new QHBoxLayout(m_playFrame);
	hLayout_frame->addWidget(m_showFrame);
	m_playFrame->setLayout(hLayout_frame);

	m_showFrame->installEventFilter(this);
	m_showLabel = new QLabel(m_showFrame);
	QTimer timer;
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateInterface));
	timer.start(1000);
}

VlcPlayWindow::~VlcPlayWindow()
{
	stopPlay();
	delete ui;
}

void VlcPlayWindow::setRtspUrl(QString rtspUrl)
{
	if (NULL != m_vlcPlayer)
	{
		stopPlay();
	}
    m_rtspUrl = rtspUrl;
}

bool VlcPlayWindow::startPlay()
{
	if (m_rtspUrl.isEmpty() || !m_rtspUrl.startsWith("rtsp://"))
	{
		printf("start play failed ,m_rtspUrl ::: %s\n",m_rtspUrl.toAscii().data());
		return false;
	}

	if( m_vlcPlayer && libvlc_media_player_is_playing(m_vlcPlayer) )
	{
		stopPlay();
	}

	/* New Media */
	_vlcMedia = libvlc_media_new_path(m_vlcObject,qtu(m_rtspUrl));
	if( NULL == _vlcMedia )
	{
		printf("start play failed ,libvlc_media_new_path failed ,m_rtspUrl ::: %s\n",
			qtu(m_rtspUrl));
		return false;
	}

	m_vlcPlayer = libvlc_media_player_new_from_media (_vlcMedia);
	if (NULL == m_vlcPlayer)
	{
		printf("start play failed ,libvlc_media_player_new_from_media\n");
		return false;
	}


	/* Integrate the video in the interface */
#if defined(Q_OS_MAC)
	libvlc_media_player_set_nsobject(m_vlcPlayer, ui->frame->winId());//m_playFrame->winId()
#elif defined(Q_OS_UNIX)
	libvlc_media_player_set_xwindow(m_vlcPlayer, ui->frame->winId());
#elif defined(Q_OS_WIN)
	libvlc_media_player_set_hwnd(m_vlcPlayer, m_playId);
#endif
    //libvlc_video_set_mouse_input(m_vlcPlayer,false);
	libvlc_video_set_key_input(m_vlcPlayer, false);
	libvlc_video_set_mouse_input(m_vlcPlayer, false);
	/* And play */
	int ret = libvlc_media_player_play (m_vlcPlayer);
	if (0 == ret)
	{
		return true;
	}
	else
	{
	    return true;
	}
}
bool VlcPlayWindow::stopPlay()
{
	if(NULL == m_vlcPlayer) 
	{
		return true;		
	}
	libvlc_media_release(_vlcMedia);
	libvlc_media_player_stop(m_vlcPlayer);
	libvlc_media_player_release(m_vlcPlayer);
	m_vlcPlayer = NULL;
	return true;
}
bool VlcPlayWindow::pause()
{
	if (libvlc_media_player_is_playing(m_vlcPlayer))
	{
		libvlc_media_player_set_pause(m_vlcPlayer,0);
	}
	return true;
}

bool VlcPlayWindow::resume()
{
	libvlc_media_player_set_pause(m_vlcPlayer,1);
	return true;
}

void VlcPlayWindow::setWindowIndex(int winIdx)
{
	m_currWinIdx = winIdx;
}
//bool VlcPlayWindow::event(QEvent *event)
//{
//if(QEvent::MouseButtonPress == event->type())
//{
//	printf("ddd");
//}
//return false;
//}

void VlcPlayWindow::mousePressEvent(QMouseEvent *event)
{
	int type = event->type();
}

void VlcPlayWindow::mouseReleaseEvent(QMouseEvent *)
{
	emit chooseWindow(m_currWinIdx);
}
bool VlcPlayWindow::eventFilter(QObject *obj, QEvent *ev)
{
	if (obj == m_playFrame)
	{
		if (ev->type() == QEvent::MouseButtonPress)
		{
			emit chooseWindow(m_currWinIdx);
			return true;
		}
	}
	if (obj == m_showFrame)
	{
		if (ev->type() == QEvent::MouseButtonPress)
		{
			emit chooseWindow(m_currWinIdx);
			 QCoreApplication::sendEvent(this,ev);
			//return true;
		}
	}
	return  QWidget::eventFilter(obj, ev); 
}

bool VlcPlayWindow::initVlc()
{
	const char *vlc_argv[32];
	int vlc_argc = 0;
	//vlc_argv[vlc_argc++] = "--no-timeshift-force";
	/* Init libVLC */
	if((m_vlcObject = libvlc_new(vlc_argc,vlc_argv)) == NULL) 
	{
		printf("Could not init libVLC");
		return false;
	}
	/* Display libVLC version */
	printf("libVLC version: %s\n",libvlc_get_version());
	return true;
}

void VlcPlayWindow::createCoreConnections()
{
	QList<libvlc_event_e> list;
	list << libvlc_MediaPlayerMediaChanged
		 << libvlc_MediaPlayerNothingSpecial
		 << libvlc_MediaPlayerOpening
		 << libvlc_MediaPlayerBuffering
		 << libvlc_MediaPlayerPlaying
		 << libvlc_MediaPlayerPaused
		 << libvlc_MediaPlayerStopped
		 << libvlc_MediaPlayerForward
		 << libvlc_MediaPlayerBackward
		 << libvlc_MediaPlayerEndReached
		 << libvlc_MediaPlayerEncounteredError
		 << libvlc_MediaPlayerTimeChanged
		 << libvlc_MediaPlayerPositionChanged
		 << libvlc_MediaPlayerSeekableChanged
		 << libvlc_MediaPlayerPausableChanged
		 << libvlc_MediaPlayerTitleChanged
		 << libvlc_MediaPlayerSnapshotTaken
		 << libvlc_MediaPlayerLengthChanged
		 << libvlc_MediaPlayerVout;
	foreach(const libvlc_event_e &event,list)
	{
		libvlc_event_attach(_vlcEvents,event,libvlc_callback,this);
	}
}

void VlcPlayWindow::removeCoreConnections()
{
	QList<libvlc_event_e> list;
	list << libvlc_MediaPlayerMediaChanged
		<< libvlc_MediaPlayerNothingSpecial
		<< libvlc_MediaPlayerOpening
		<< libvlc_MediaPlayerBuffering
		<< libvlc_MediaPlayerPlaying
		<< libvlc_MediaPlayerPaused
		<< libvlc_MediaPlayerStopped
		<< libvlc_MediaPlayerForward
		<< libvlc_MediaPlayerBackward
		<< libvlc_MediaPlayerEndReached
		<< libvlc_MediaPlayerEncounteredError
		<< libvlc_MediaPlayerTimeChanged
		<< libvlc_MediaPlayerPositionChanged
		<< libvlc_MediaPlayerSeekableChanged
		<< libvlc_MediaPlayerPausableChanged
		<< libvlc_MediaPlayerTitleChanged
		<< libvlc_MediaPlayerSnapshotTaken
		<< libvlc_MediaPlayerLengthChanged
		<< libvlc_MediaPlayerVout;
	foreach(const libvlc_event_e &event,list)
	{
		libvlc_event_detach(_vlcEvents,event,libvlc_callback,this);
	}
}

void VlcPlayWindow::libvlc_callback(const libvlc_event_t *event,void *data)
{
	VlcPlayWindow *core = (VlcPlayWindow *)data;
	printf("libvlc_callback ,event type:%d\n",event->type);
	switch (event->type)
	{
		case libvlc_MediaPlayerMediaChanged:
			emit core->mediaChanged(event->u.media_player_media_changed.new_media);
			break;
		case libvlc_MediaPlayerNothingSpecial:
			emit core->nothingSpecial();
			break;
		case libvlc_MediaPlayerOpening:
			emit core->opening();
			break;
		case libvlc_MediaPlayerBuffering:
			emit core->buffering(event->u.media_player_buffering.new_cache);
		case libvlc_MediaPlayerPlaying:
			emit core->playing();
			break;
		case libvlc_MediaPlayerPaused:
			emit core->paused();
			break;
		case libvlc_MediaPlayerStopped:
			emit core->stopped();
			break;
		case libvlc_MediaPlayerForward:
			emit core->forward();
			break;
		case libvlc_MediaPlayerBackward:
			emit core->backward();
			break;
		case libvlc_MediaPlayerEndReached:
			emit core->end();
			break;
		case libvlc_MediaPlayerEncounteredError:
			emit core->error();
			break;
		case libvlc_MediaPlayerTimeChanged:
			emit core->timeChanged(event->u.media_player_time_changed.new_time);
			break;
		case libvlc_MediaPlayerPositionChanged:
			emit core->positionChanged(event->u.media_player_position_changed.new_position);
			break;
		case libvlc_MediaPlayerSeekableChanged:
			emit core->seekableChanged(event->u.media_player_seekable_changed.new_seekable);
			break;
		case libvlc_MediaPlayerPausableChanged:
			emit core->pausableChanged(event->u.media_player_pausable_changed.new_pausable);
			break;
		case libvlc_MediaPlayerTitleChanged:
			emit core->titleChanged(event->u.media_player_title_changed.new_title);
			break;
		case libvlc_MediaPlayerSnapshotTaken:
			emit core->snapshotTaken(event->u.media_player_snapshot_taken.psz_filename);
			break;
		case libvlc_MediaPlayerLengthChanged:
			emit core->lengthChanged(event->u.media_player_length_changed.new_length);
			break;
		case libvlc_MediaPlayerVout:
			emit core->vout(event->u.media_player_vout.new_count);
			break;
		default:
			break;
		}

		if (event->type >= libvlc_MediaPlayerNothingSpecial &&
			event->type <= libvlc_MediaPlayerEncounteredError) 
		{
				emit core->stateChanged();
		}
}

void VlcPlayWindow::updateInterface() { //Update interface and check if song is finished

	if(m_vlcPlayer) //It segfault if vlcPlayer don't exist
	{
		/* Stop the media */
		int vlcState = libvlc_media_player_get_state(m_vlcPlayer);

		if (libvlc_Ended == vlcState) 
		{ 
			 m_showLabel->setText("libvlc_Ended");
		}
		switch (vlcState)
		{
		case libvlc_Buffering:
			m_showLabel->setText("libvlc_Buffering");
			break;
		case libvlc_Ended:
			m_showLabel->setText("libvlc_Ended");
			break;
		case libvlc_NothingSpecial:
			m_showLabel->setText("libvlc_NothingSpecial");
			break;
		case libvlc_Opening:
			m_showLabel->setText("libvlc_Opening");
			break;
		case libvlc_Playing:
			m_showLabel->setText("libvlc_Playing");
			break;
		case libvlc_Paused:
			m_showLabel->setText("libvlc_Paused");
			break;
		case libvlc_Stopped:
			m_showLabel->setText("libvlc_Stopped");
			break;
		case libvlc_Error:
			m_showLabel->setText("libvlc_Error");
			break;
		default:
			QString otherStatus = QString("other status:%1").arg(vlcState);
			m_showLabel->setText(otherStatus);
			break;
		}

	}
}

QString VlcPlayWindow::record(const QString &name,
						 const QString &path,
						 const Vlc::Mux &mux,
						 bool duplicate)
{
	QString option1, option2, parameters;
	QString l = QDir::toNativeSeparators(path + "/" + name);

	parameters = "std{access=file,mux=%1,dst='%2'}";
	parameters = parameters.arg(Vlc::mux()[mux], l + "." + Vlc::mux()[mux]);

	if (duplicate) {
		option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
	} else {
		option2 = ":sout=#%1";
	}

	option1 = ":sout-all";
	option2 = option2.arg(parameters);

	setOption(option1);
	setOption(option2);

	//VlcError::showErrmsg();

	return l + "." + Vlc::mux()[mux];
}

QString VlcPlayWindow::record(const QString &name,
						 const QString &path,
						 const Vlc::Mux &mux,
						 const Vlc::AudioCodec &audioCodec,
						 const Vlc::VideoCodec &videoCodec,
						 bool duplicate)
{
	QString option1, option2, parameters;
	QString l = QDir::toNativeSeparators(path + "/" + name);

	parameters = "transcode{vcodec=%1,acodec=%2}:std{access=file,mux=%3,dst='%4'}";
	parameters = parameters.arg(Vlc::videoCodec()[videoCodec], Vlc::audioCodec()[audioCodec], Vlc::mux()[mux], l + "." + Vlc::mux()[mux]);

	if (duplicate) {
		option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
	} else {
		option2 = ":sout=#%1";
	}

	option1 = ":sout-all";
	option2 = option2.arg(parameters);

	setOption(option1);
	setOption(option2);

	//VlcError::showErrmsg();

	return l + "." + Vlc::mux()[mux];
}

QString VlcPlayWindow::record(const QString &name,
						 const QString &path,
						 const Vlc::Mux &mux,
						 const Vlc::AudioCodec &audioCodec,
						 const Vlc::VideoCodec &videoCodec,
						 int bitrate,
						 int fps,
						 int scale,
						 bool duplicate)
{
	QString option1, option2, parameters;
	QString l = QDir::toNativeSeparators(path + "/" + name);

	parameters = "transcode{vcodec=%1,vb=%2,fps=%3,scale=%4,acodec=%5}:std{access=file,mux=%6,dst='%7'}";
	parameters = parameters.arg(Vlc::videoCodec()[videoCodec], QString::number(bitrate), QString::number(fps), QString::number(scale), Vlc::audioCodec()[audioCodec], Vlc::mux()[mux], l + "." + Vlc::mux()[mux]);

	if (duplicate) {
		option2 = ":sout=#duplicate{dst=display,dst=\"%1\"}";
	} else {
		option2 = ":sout=#%1";
	}

	option1 = ":sout-all";
	option2 = option2.arg(parameters);

	setOption(option1);
	setOption(option2);

	//VlcError::showErrmsg();

	return l + "." + Vlc::mux()[mux];
}

void VlcPlayWindow::setProgram(int program)
{
	QString option = "program=%1";
	setOption(option.arg(program));
}

void VlcPlayWindow::setOption(const QString &option)
{
	libvlc_media_add_option(_vlcMedia, option.toUtf8().data());

	//VlcError::showErrmsg();
}

void VlcPlayWindow::setOptions(const QStringList &options)
{
	foreach(const QString &option, options) 
	{
		libvlc_media_add_option(_vlcMedia, option.toUtf8().data());
	}

	//VlcError::showErrmsg();
}