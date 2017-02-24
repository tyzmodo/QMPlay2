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

#include <Demuxer.hpp>

class ToneGenerator : public Demuxer
{
	Q_DECLARE_TR_FUNCTIONS(ToneGenerator)
public:
	ToneGenerator(Module &);

	bool set() override final;
private:
	bool metadataChanged() const override final;

	QString name() const override final;
	QString title() const override final;
	double length() const override final;
	int bitrate() const override final;

	bool dontUseBuffer() const override final;

	bool seek(int, bool) override final;
	bool read(Packet &, int &) override final;
	void abort() override final;

	bool open(const QString &) override final;

	/**/

	volatile bool aborted;
	mutable volatile bool metadata_changed;
	bool fromUrl;
	double pos;
	quint32 srate;
	QVector<quint32> freqs;
};

#define ToneGeneratorName "ToneGenerator"
