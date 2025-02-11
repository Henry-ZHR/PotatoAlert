// Copyright 2020 <github.com/razaqq>

#include "Core/Directory.hpp"
#include "Core/Format.hpp"
#include "Core/Log.hpp"
#include "Core/Screenshot.hpp"
#include "Core/Time.hpp"

#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

#include <filesystem>
#include <string>


namespace fs = std::filesystem;

static constexpr std::string_view g_timeFormat = "%Y-%m-%d_%H-%M-%S";

namespace {

static std::string GetFileName()
{
	return fmt::format("capture_{}.png", PotatoAlert::Core::Time::GetTimeStamp(g_timeFormat));
}

}

bool PotatoAlert::Core::CaptureScreenshot(QWidget* window, const fs::path& dir, const QList<QRect>& blurRects)
{
	if (!window)
		return false;

	QPixmap pix(window->size());
	window->render(&pix);

	if (!blurRects.empty())
	{
		QGraphicsScene* scene = new QGraphicsScene();
		scene->addPixmap(pix);

		QGraphicsBlurEffect* effect = new QGraphicsBlurEffect();
		effect->setBlurHints(QGraphicsBlurEffect::BlurHint::QualityHint);
		effect->setBlurRadius(15);

		QGraphicsScene* blurScene = new QGraphicsScene();
		QGraphicsPixmapItem* pixItem = blurScene->addPixmap(pix);
		pixItem->setGraphicsEffect(effect);
		QGraphicsView* blurView = new QGraphicsView(blurScene);
		blurView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		blurView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		const QPixmap blurPix = blurView->grab(blurView->sceneRect().toRect());

		for (const QRect& r : blurRects)
		{
			QGraphicsPixmapItem* item = scene->addPixmap(blurPix.copy(r));
			item->setOffset(r.topLeft());
		}

		QGraphicsView* view = new QGraphicsView(scene);
		view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		pix = view->grab(view->sceneRect().toRect());
	}

	const std::string fileName = GetFileName();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	if (pix.save(QDir(dir).absoluteFilePath(fileName.c_str()), "PNG", 100))
#else
	if (pix.save(FromFilesystemPath(dir).absoluteFilePath(fileName.c_str()), "PNG", 100))
#endif
	{
		LOG_TRACE("Saved screenshot {}", fileName);
		return true;
	}
	LOG_ERROR("Failed to save screenshot.");
	return false;
}
