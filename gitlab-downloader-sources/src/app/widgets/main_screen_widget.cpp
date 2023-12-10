#include "main_screen_widget.hpp"
#include <QListView>
#include <QPushButton>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>

MainScreenWidget::MainScreenWidget(QWidget *parent) : QWidget(parent) {

  setLayout(createRootLayout());
}

QLayout *MainScreenWidget::createRootLayout() {
  const auto rootLayout = [this]() {
    const auto layout = new QVBoxLayout();

    const auto listView = new QListView();

    layout->addLayout(createTopLayout());
    layout->addWidget(listView);
    layout->addLayout(createBottomLayout());

    return layout;
  }();

  return rootLayout;
}

QLayout *MainScreenWidget::createTopLayout() {
  const auto topLayout = [this]() {
    const auto layout = new QHBoxLayout();
    const auto labelStatus =
        new QLabel("Current status:\nHere-Will-Be-Current-Status");
    labelStatus->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    const auto refresedLastTimeLabel =
        new QLabel("Refreshed last time:\nHere-Will-Be-Refreshed-Last-Time");
    refresedLastTimeLabel->setSizePolicy(QSizePolicy::Minimum,
                                         QSizePolicy::Minimum);
    const auto settingsButton = new QPushButton("Settings");
    settingsButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    layout->addWidget(labelStatus, 0, Qt::AlignLeft);
    layout->addWidget(refresedLastTimeLabel, 0, Qt::AlignLeft);
    layout->addStretch();
    layout->addWidget(settingsButton, 0, Qt::AlignRight);

    QObject::connect(settingsButton, &QPushButton::clicked, this,
                     &MainScreenWidget::settingsButtonClicked);

    return layout;
  }();

  return topLayout;
}

QLayout *MainScreenWidget::createBottomLayout() {
  const auto bottomLayout = []() {
    const auto layout = new QHBoxLayout();
    const auto refreshAllButton = new QPushButton("Refresh all");
    const auto downloadNewArtifactsButton =
        new QPushButton("Download New Artifacts");

    layout->addStretch();
    layout->addWidget(refreshAllButton);
    layout->addWidget(downloadNewArtifactsButton);

    return layout;
  }();

  return bottomLayout;
}
