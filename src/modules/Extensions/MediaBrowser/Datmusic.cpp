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

#include <MediaBrowser/Datmusic.hpp>

#include <QMPlay2Extensions.hpp>
#include <NetworkAccess.hpp>
#include <Functions.hpp>
#include <Json11.hpp>

#include <QHeaderView>
#include <QTreeWidget>
#include <QAction>

constexpr char g_url[]  = "http://api.datmusic.xyz";
constexpr char g_referUrlRawHeader[] = "Referer: https://datmusic.xyz/\r\n";

/**/

Datmusic::Datmusic(NetworkAccess &net) :
	MediaBrowserCommon(net, "Datmusic", ":/applications-multimedia.svgz")
{}
Datmusic::~Datmusic()
{}

void Datmusic::prepareWidget(QTreeWidget *treeW)
{
	MediaBrowserCommon::prepareWidget(treeW);

	treeW->sortByColumn(0, Qt::AscendingOrder);

	treeW->headerItem()->setText(0, tr("Title"));
	treeW->headerItem()->setText(1, tr("Artist"));
	treeW->headerItem()->setText(2, tr("Length"));

	Functions::setHeaderSectionResizeMode(treeW->header(), 2, QHeaderView::ResizeToContents);
}

QString Datmusic::getQMPlay2Url(const QString &text) const
{
	return QString("%1://{%2}").arg(m_name, text);
}

NetworkReply *Datmusic::getSearchReply(const QString &text, const qint32 page)
{
	return m_net.start(QString("%1/search?q=%2&page=%3").arg(g_url, text).arg(page - 1), QByteArray(), g_referUrlRawHeader);
}
MediaBrowserCommon::Description Datmusic::addSearchResults(const QByteArray &reply, QTreeWidget *treeW)
{
	const QIcon datmusicIcon = icon();

	const Json::array jsonArray = Json::parse(reply)["data"].array_items();

	for (size_t i = 0; i < jsonArray.size(); ++i)
	{
		Json::object entry = jsonArray[i].object_items();
		if (entry.empty())
			continue;

		QTreeWidgetItem *tWI = new QTreeWidgetItem(treeW);
		tWI->setData(0, Qt::UserRole, entry["download"].string_value());
		tWI->setIcon(0, datmusicIcon);

		tWI->setText(0, entry["title"].string_value());
		tWI->setToolTip(0, tWI->text(0));

		tWI->setText(1, entry["artist"].string_value());
		tWI->setToolTip(1, tWI->text(1));

		tWI->setText(2, Functions::timeToStr(entry["duration"].int_value()));
	}

	return {};
}

MediaBrowserCommon::PagesMode Datmusic::pagesMode() const
{
	return PagesMode::Multi;
}

bool Datmusic::hasWebpage() const
{
	return false;
}
QString Datmusic::getWebpageUrl(const QString &text) const
{
	Q_UNUSED(text)
	return QString();
}

MediaBrowserCommon::CompleterMode Datmusic::completerMode() const
{
	return CompleterMode::Continuous;
}
NetworkReply *Datmusic::getCompleterReply(const QString &text)
{
	return m_net.start("https://my-free-mp3.net/api/autocomplete.php", "query=" + text.toUtf8().toPercentEncoding(), NetworkAccess::UrlEncoded);
}
QStringList Datmusic::getCompletions(const QByteArray &reply)
{
	const Json::array jsonArray = Json::parse(reply).array_items();
	QStringList completions;
	for (size_t i = 0; i < jsonArray.size(); ++i)
	{
		const QString name = jsonArray[i]["name"].string_value();
		if (!name.isEmpty())
			completions += name;
	}
	return completions;
}

QAction *Datmusic::getAction() const
{
	QAction *act = new QAction(tr("Search on Datmusic"), nullptr);
	act->setIcon(icon());
	return act;
}

bool Datmusic::convertAddress(const QString &prefix, const QString &url, const QString &param, QString *streamUrl, QString *name, QIcon *icon, QString *extension, IOController<> *ioCtrl)
{
	Q_UNUSED(param)
	Q_UNUSED(name)

	if (prefix != m_name)
		return false;

	if (streamUrl || icon)
	{
#if 0 // Icon needed
		if (icon)
			*icon = m_icon;
#endif
		if (extension)
			*extension = ".mp3";
		if (ioCtrl && streamUrl)
		{
			QMPlay2Core.addRawHeaders(url, g_referUrlRawHeader);
			*streamUrl = url;
		}
	}
	return true;
}
