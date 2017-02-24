/*
	QMPlay2 is a video and audio player.
	Copyright (C) 2010-2017  Błażej Szczygieł

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <PlayClass.hpp>
#ifdef UPDATER
	#include <Updater.hpp>
#endif

class QFrame;
class QLabel;
class Slider;
class MenuBar;
class InfoDock;
class VideoDock;
class VolWidget;
class IPCSocket;
class QToolButton;
class AboutWidget;
class PlaylistDock;
class SettingsWidget;
class QMPlay2Extensions;

class MainWidget : public QMainWindow
{
	friend class QMPlay2GUIClass;
	Q_PROPERTY(bool fullScreen READ getFullScreen)
	Q_OBJECT
public:
	MainWidget(QPair<QStringList, QStringList> &);
	~MainWidget();
private slots:
	void detachFromPipe();

	void focusChanged(QWidget *, QWidget *);

	void processParam(const QString &, const QString &);

	void audioChannelsChanged();

	void updateWindowTitle(const QString &t = QString());
	void videoStarted();

	void togglePlay();
	void seek(int);
	void playStateChanged(bool);

	void volUpDown();
	void toggleMuteIcon();
	void actionSeek();
	void switchARatio();
	void resetARatio();
	void resetFlip();
	void resetRotate90();

	void visualizationFullScreen();
	void hideAllExtensions();
	void toggleVisibility();
	void createMenuBar();
	void trayIconClicked(QSystemTrayIcon::ActivationReason);
	void toggleCompactView();
	void toggleFullScreen();
	void showMessage(const QString &, const QString &, int, int);
	void statusBarMessage(const QString &, int ms);

	void openUrl();
	void openFiles();
	void openDir();
	void loadPlist();
	void savePlist();
	void saveGroup();
	void showSettings(const QString &);
	void showSettings();

	void itemDropped(const QString &, bool);
	void browseSubsFile();

	void setSeekSMaximum(int);
	void updatePos(int);
	void mousePositionOnSlider(int);

	void newConnection(IPCSocket *socket);
	void readSocket();

	void about();

#if !defined Q_OS_MAC && !defined Q_OS_ANDROID
	void hideMenu(bool);
#else
	void hideMenu(bool) {} //To avoid incorrect CPP file generated by MOC on Android
#endif
	void lockWidgets(bool);

	void hideDocksSlot();
	void delayedRestore(QByteArray data);

	void uncheckSuspend();
private:
	void savePlistHelper(const QString &, const QString &, bool);

	QMenu *createPopupMenu() override final;

	void showToolBar(bool);
	void hideDocks();
	void restoreFocus();

	inline bool isTrayVisible() const;

	bool getFullScreen() const;

	void keyPressEvent(QKeyEvent *) override final;
	void mouseMoveEvent(QMouseEvent *) override final;
	void leaveEvent(QEvent *) override final;
	void closeEvent(QCloseEvent *) override final;
	void changeEvent(QEvent *) override final;
	void moveEvent(QMoveEvent *) override final;
	void showEvent(QShowEvent *) override final;
	void hideEvent(QHideEvent *) override final;

#ifdef Q_OS_MAC
	bool eventFilter(QObject *obj, QEvent *event) override final;

	void fileOpenTimerTimeout();

	QTimer fileOpenTimer;
	QStringList filesToAdd;
#endif

	MenuBar *menuBar;
	QToolBar *mainTB;
	QStatusBar *statusBar;

	QFrame *vLine;
	QLabel *timeL;

	VideoDock *videoDock;
	InfoDock *infoDock;
	PlaylistDock *playlistDock;

	Slider *seekS;
	VolWidget *volW;

	PlayClass playC;

	QSystemTrayIcon *tray;

	QByteArray dockWidgetState, fullScreenDockWidgetState;
	QList<QMPlay2Extensions *> visibleQMPlay2Extensions;
#ifndef Q_OS_ANDROID
	QRect savedGeo;
#endif

	SettingsWidget *settingsW;
	AboutWidget *aboutW;
	QWidget *lastFocusWidget;
	bool isCompactView, wasShow, fullScreen, seekSFocus;

#if !defined Q_OS_MAC && !defined Q_OS_ANDROID
	QAction *hideMenuAct;
#endif
	QAction *lockWidgetsAct;

#ifdef UPDATER
	Updater updater;
#endif
};
