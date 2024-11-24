#ifndef HISTOGRAM_WINDOW_H
#define HISTOGRAM_WINDOW_H
#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QHeaderView>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include "information_processing/Histogram.h"

class HistogramWindow : public QMainWindow {
public:
    HistogramWindow(QWidget *parent = nullptr)
            : QMainWindow(parent), dataTable(new QTableWidget), rangeTable(new QTableWidget), chartView(new QChartView) {

        // Основной виджет и макет
        auto *centralWidget = new QWidget;
        auto *mainLayout = new QVBoxLayout;

        // Меню
        setupMenu();

        // Поля ввода и кнопки
        setupInputSection(mainLayout);

        // Виджет для гистограммы
        chartView->setRenderHint(QPainter::Antialiasing);
        mainLayout->addWidget(chartView);

        // Устанавливаем основной виджет
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);

        // Устанавливаем пример данных
        populateTableWithExampleData();
    }

private slots:
    void onBuildHistogram() {
        // Считываем данные
        ArraySequence<int> data;
        for (int i = 0; i < dataTable->rowCount(); ++i) {
            auto item = dataTable->item(i, 0);
            if (item) {
                data.append(item->text().toInt());
            }
        }

        // Считываем диапазоны
        ArraySequence<Histogram<int, QString>::Range> ranges;
        for (int i = 0; i < rangeTable->rowCount(); ++i) {
            auto startItem = rangeTable->item(i, 0);
            auto endItem = rangeTable->item(i, 1);
            if (startItem && endItem) {
                ranges.append({startItem->text().toInt(), endItem->text().toInt()});
            }
        }

        // Проверка на пустоту
        if (data.getLength() == 0 || ranges.getLength() == 0) {
            QMessageBox::warning(this, "Ошибка", "Данные или диапазоны не заданы!");
            return;
        }

        // Выбор классификации
        auto classificationRule = classificationCombo->currentText();
        auto classifier = [classificationRule](const int &value) -> QString {
            if (classificationRule == "Чётное/Нечётное") {
                return value % 2 == 0 ? "Чётное" : "Нечётное";
            } else if (classificationRule == "Больше/Меньше 5") {
                return value > 5 ? "Больше 5" : "Меньше или равно 5";
            }
            return "Неизвестно";
        };

        // Создаём гистограмму
        Histogram<int, QString> histogram(
                data,
                ranges,
                [](const int &value) { return value; }, // Критерий
                classifier);

        // Отображение гистограммы
        displayHistogram(histogram);
    }

    void onAddRowToData() {
        dataTable->insertRow(dataTable->rowCount());
    }

    void onAddRowToRanges() {
        rangeTable->insertRow(rangeTable->rowCount());
    }

    void onSaveData() {
        QString fileName = QFileDialog::getSaveFileName(this, "Сохранить данные", "", "Текстовые файлы (*.txt)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream out(&file);
                for (int i = 0; i < dataTable->rowCount(); ++i) {
                    auto item = dataTable->item(i, 0);
                    if (item) {
                        out << item->text() << "\n";
                    }
                }
                file.close();
            }
        }
    }

    void onLoadData() {
        QString fileName = QFileDialog::getOpenFileName(this, "Загрузить данные", "", "Текстовые файлы (*.txt)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly)) {
                QTextStream in(&file);
                dataTable->clearContents();
                int row = 0;
                while (!in.atEnd()) {
                    if (dataTable->rowCount() <= row) {
                        dataTable->insertRow(row);
                    }
                    dataTable->setItem(row, 0, new QTableWidgetItem(in.readLine()));
                    ++row;
                }
                file.close();
            }
        }
    }

private:
    void setupMenu() {
        auto *fileMenu = menuBar()->addMenu("Файл");

        auto *loadAction = new QAction("Загрузить данные", this);
        connect(loadAction, &QAction::triggered, this, &HistogramWindow::onLoadData);
        fileMenu->addAction(loadAction);

        auto *saveAction = new QAction("Сохранить данные", this);
        connect(saveAction, &QAction::triggered, this, &HistogramWindow::onSaveData);
        fileMenu->addAction(saveAction);
    }

    void setupInputSection(QVBoxLayout *layout) {
        // Группа для данных
        auto *dataGroup = new QGroupBox("Данные");
        auto *dataLayout = new QVBoxLayout;
        dataTable->setColumnCount(1);
        dataTable->setHorizontalHeaderLabels({"Value"});
        dataTable->horizontalHeader()->setStretchLastSection(true);
        dataLayout->addWidget(dataTable);

        auto *addDataButton = new QPushButton("Добавить строку");
        connect(addDataButton, &QPushButton::clicked, this, &HistogramWindow::onAddRowToData);
        dataLayout->addWidget(addDataButton);
        dataGroup->setLayout(dataLayout);
        layout->addWidget(dataGroup);

        // Группа для диапазонов
        auto *rangeGroup = new QGroupBox("Диапазоны");
        auto *rangeLayout = new QVBoxLayout;
        rangeTable->setColumnCount(2);
        rangeTable->setHorizontalHeaderLabels({"Start", "End"});
        rangeTable->horizontalHeader()->setStretchLastSection(true);
        rangeLayout->addWidget(rangeTable);

        auto *addRangeButton = new QPushButton("Добавить диапазон");
        connect(addRangeButton, &QPushButton::clicked, this, &HistogramWindow::onAddRowToRanges);
        rangeLayout->addWidget(addRangeButton);
        rangeGroup->setLayout(rangeLayout);
        layout->addWidget(rangeGroup);

        // Группа для классификации
        auto *classificationGroup = new QGroupBox("Классификация");
        auto *classificationLayout = new QFormLayout;
        classificationCombo = new QComboBox;
        classificationCombo->addItem("Чётное/Нечётное");
        classificationCombo->addItem("Больше/Меньше 5");
        classificationLayout->addRow("Правило:", classificationCombo);
        classificationGroup->setLayout(classificationLayout);
        layout->addWidget(classificationGroup);

        // Кнопка для построения гистограммы
        auto *buildButton = new QPushButton("Построить гистограмму");
        connect(buildButton, &QPushButton::clicked, this, &HistogramWindow::onBuildHistogram);
        layout->addWidget(buildButton);
    }

    void populateTableWithExampleData() {
        dataTable->setRowCount(5);
        for (int i = 0; i < 5; ++i) {
            dataTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        }

        rangeTable->setRowCount(2);
        rangeTable->setItem(0, 0, new QTableWidgetItem("0"));
        rangeTable->setItem(0, 1, new QTableWidgetItem("3"));
        rangeTable->setItem(1, 0, new QTableWidgetItem("3"));
        rangeTable->setItem(1, 1, new QTableWidgetItem("6"));
    }

    void displayHistogram(const Histogram<int, QString> &histogram) {
        // Создаем новый объект QChart
        auto *chart = new QChart;
        chart->setTitle("Гистограмма данных");
        chart->legend()->setAlignment(Qt::AlignBottom);

        bool hasData = false; // Флаг для проверки наличия данных

        auto rangeKeys = histogram.getHistogram().GetKeys();
        for (int i = 0; i < rangeKeys.getLength(); ++i) {
            const auto &range = rangeKeys[i];
            const auto &classStats = histogram.getHistogram().GetReference(range);

            auto classKeys = classStats.GetKeys();
            for (int j = 0; j < classKeys.getLength(); ++j) {
                const auto &className = classKeys[j];
                const auto &stats = classStats.GetReference(className);

                // Создаем новый набор данных для каждого диапазона и класса
                auto *set = new QBarSet(QString("[%1, %2) %3").arg(range.first).arg(range.second).arg(className));
                *set << stats.count;

                // Добавляем данные в серию
                auto *series = new QBarSeries;
                series->append(set);
                chart->addSeries(series);

                hasData = true; // Данные добавлены
            }
        }

        // Проверка, есть ли данные для отображения
        if (!hasData) {
            QMessageBox::warning(this, "Ошибка", "Нет данных для построения гистограммы.");
            return;
        }

        // Создаем оси и добавляем их к графику
        chart->createDefaultAxes();
        chart->axes(Qt::Vertical).first()->setTitleText("Количество");
        chart->axes(Qt::Horizontal).first()->setTitleText("Классы и диапазоны");

        // Устанавливаем созданный график в chartView
        chartView->setChart(chart);
        chartView->update();
    }

    QTableWidget *dataTable;
    QTableWidget *rangeTable;
    QComboBox *classificationCombo;
    QChartView *chartView;
};
#endif // HISTOGRAM_WINDOW_H
