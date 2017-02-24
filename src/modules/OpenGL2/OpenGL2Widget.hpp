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

#include <OpenGL2CommonQt5.hpp>

#include <QSurfaceFormat>
#include <QOpenGLWidget>

class OpenGL2Widget : public QOpenGLWidget, public OpenGL2CommonQt5
{
	Q_OBJECT
public:
	OpenGL2Widget();
	~OpenGL2Widget();

	QWidget *widget() override final;

	bool setVSync(bool enable) override final;
	void updateGL(bool requestDelayed) override final;

	void initializeGL() override final;
	void paintGL() override final;
private slots:
	void aboutToBeDestroyed();
private:
	bool event(QEvent *e) override final;
};
