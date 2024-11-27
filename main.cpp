#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QtCharts>
#include <QInputDialog>
#include <QMessageBox>
#include <vector>
#include <string>
#include "sequence/ArraySequence.h"
#include "information_processing/Index.h"
#include "information_processing/Histogram.h"


struct Person {
    QString name;
    QString city;
    int age;
    double salary;

    Person() : age(0), salary(0.0) {}
    Person(const QString& n, const QString& c, int a, double s)
            : name(n), city(c), age(a), salary(s) {}
};

class MainWindow : public QMainWindow {


public:
    MainWindow(QWidget* parent = nullptr)
            : QMainWindow(parent),
              compositeIndex(people, std::make_tuple(
                      [](const Person& p) -> QString { return p.name; },
                      [](const Person& p) -> QString { return p.city; }
              ))
    {
        setWindowTitle("Data Analysis Tool");
        resize(1200, 800);

        // Создание центрального виджета и его расположения
        auto* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        auto* mainLayout = new QVBoxLayout(centralWidget);

        // Создание вкладок
        auto* tabWidget = new QTabWidget(this);
        mainLayout->addWidget(tabWidget);

        // Создание вкладок
        createDataTab(tabWidget);
        createHistogramTab(tabWidget);
        createIndexTab(tabWidget);

        // Инициализация данных
        initializeData();
    }

private:
    QTableWidget* dataTable;       // Таблица данных
    QChartView* histogramView;     // Виджет гистограммы
    QTableWidget* indexTable;      // Таблица результатов поиска и индекса
    QTableWidget* allIndexTable;   // Таблица всех записей индекса
    ArraySequence<Person> people;  // Последовательность людей
    Index<Person, QString, QString> compositeIndex; // Композитный индекс

    // Метод создания вкладки данных
    void createDataTab(QTabWidget* tabWidget) {
        auto* dataWidget = new QWidget();
        auto* layout = new QVBoxLayout(dataWidget);

        // Таблица данных
        dataTable = new QTableWidget();
        dataTable->setColumnCount(4);
        dataTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});
        layout->addWidget(dataTable);

        // Кнопки добавления и удаления
        auto* buttonLayout = new QHBoxLayout();
        auto* addButton = new QPushButton("Add Person");
        auto* removeButton = new QPushButton("Remove Selected");
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(removeButton);
        layout->addLayout(buttonLayout);

        // Связывание сигналов и слотов
        connect(addButton, &QPushButton::clicked, this, &MainWindow::addPerson);
        connect(removeButton, &QPushButton::clicked, this, &MainWindow::removePerson);

        tabWidget->addTab(dataWidget, "Data");
    }

    // Метод создания вкладки гистограммы
    void createHistogramTab(QTabWidget* tabWidget) {
        auto* histogramWidget = new QWidget();
        auto* layout = new QVBoxLayout(histogramWidget);

        // Контролы для выбора критериев и генерации гистограммы
        auto* controlLayout = new QHBoxLayout();
        auto* criteriaCombo = new QComboBox();
        criteriaCombo->addItems({"Age", "Salary"});
        auto* generateButton = new QPushButton("Generate Histogram");
        controlLayout->addWidget(new QLabel("Criteria:"));
        controlLayout->addWidget(criteriaCombo);
        controlLayout->addWidget(generateButton);
        layout->addLayout(controlLayout);

        // Виджет отображения гистограммы
        histogramView = new QChartView();
        histogramView->setRenderHint(QPainter::Antialiasing);
        layout->addWidget(histogramView);

        // Связывание сигналов и слотов
        connect(generateButton, &QPushButton::clicked, [this, criteriaCombo]() {
            generateHistogram(criteriaCombo->currentText());
        });

        tabWidget->addTab(histogramWidget, "Histogram");
    }

    // Метод создания вкладки индекса
    void createIndexTab(QTabWidget* tabWidget) {
        auto* indexWidget = new QWidget();
        auto* layout = new QVBoxLayout(indexWidget);

        // Группа контролов для поиска
        auto* searchGroupLayout = new QHBoxLayout();
        auto* nameLabel = new QLabel("Name:");
        auto* nameField = new QLineEdit();
        auto* cityLabel = new QLabel("City:");
        auto* cityField = new QLineEdit();
        auto* searchButton = new QPushButton("Search");

        searchGroupLayout->addWidget(nameLabel);
        searchGroupLayout->addWidget(nameField);
        searchGroupLayout->addWidget(cityLabel);
        searchGroupLayout->addWidget(cityField);
        searchGroupLayout->addWidget(searchButton);
        layout->addLayout(searchGroupLayout);

        // Таблица для отображения результатов поиска
        indexTable = new QTableWidget();
        indexTable->setColumnCount(4);
        indexTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});
        layout->addWidget(new QLabel("Search Results:"));
        layout->addWidget(indexTable);

        // Кнопка для отображения всех записей индекса
        auto* showAllButton = new QPushButton("Show All Index Entries");
        layout->addWidget(showAllButton);

        // Таблица для отображения всех записей индекса
        allIndexTable = new QTableWidget();
        allIndexTable->setColumnCount(4);
        allIndexTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});
        layout->addWidget(new QLabel("All Index Entries:"));
        layout->addWidget(allIndexTable);

        // Связывание сигналов и слотов
        connect(searchButton, &QPushButton::clicked, [this, nameField, cityField]() {
            QString nameQuery = nameField->text();
            QString cityQuery = cityField->text();
            searchIndex(nameQuery, cityQuery);
        });

        connect(showAllButton, &QPushButton::clicked, this, &MainWindow::showAllIndexEntries);

        tabWidget->addTab(indexWidget, "Index");
    }

    // Метод генерации гистограммы
    void generateHistogram(const QString& criteria) {
        ArraySequence<std::pair<double, double>> ranges;
        if (criteria == "Age") {
            ranges.append({0, 20});
            ranges.append({20, 40});
            ranges.append({40, 60});
            ranges.append({60, 100});
        } else { // Salary
            ranges.append({0, 50000});
            ranges.append({50000, 100000});
            ranges.append({100000, 150000});
            ranges.append({150000, 200000});
        }

        Histogram<double, QString, Person> histogram(
                people,
                ranges,
                [criteria](const Person& p) -> double {
                    return criteria == "Age" ? static_cast<double>(p.age) : p.salary;
                },
                [](const Person& p) -> QString {
                    return p.city;
                }
        );

        updateHistogramChart(histogram);
    }

    // Метод обновления гистограммы
    void updateHistogramChart(const Histogram<double, QString, Person>& histogram) {
        auto* series = new QBarSeries();
        const auto& histData = histogram.getHistogram();

        QStringList cities;
        ArraySequence<std::pair<double, double>> ranges = histData.GetKeys();
        for (int i = 0; i < ranges.getLength(); i++) {
            const auto& range = ranges[i];
            auto* set = new QBarSet(QString("%1-%2").arg(range.first).arg(range.second));
            const auto& classData = histData.GetReference(range);

            ArraySequence<QString> classKeys = classData.GetKeys();
            for (int j = 0; j < classKeys.getLength(); j++) {
                const QString& city = classKeys[j];
                if (!cities.contains(city)) cities << city;
                *set << classData.GetReference(city).count;
            }
            series->append(set);
        }

        auto* chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Distribution by Range and City");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Создание осей
        auto* axisX = new QBarCategoryAxis();
        axisX->append(cities);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        auto* axisY = new QValueAxis();
        axisY->setRange(0, 25);
        axisY->setTitleText("Count");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Установка графика в виджет
        histogramView->setChart(chart);
    }

    // Метод поиска в индексе по заданным критериям
    void searchIndex(const QString& nameQuery, const QString& cityQuery) {
        ArraySequence<std::tuple<QString, QString>> keys = compositeIndex.GetAllKeys();
        ArraySequence<Person> values = compositeIndex.GetAllValues();
        ArraySequence<Person> results;

        for (int i = 0; i < keys.getLength(); ++i) {
            const auto& key = keys[i]; // std::tuple<QString, QString>
            bool nameMatch = nameQuery.isEmpty() || std::get<0>(key).contains(nameQuery, Qt::CaseInsensitive);
            bool cityMatch = cityQuery.isEmpty() || std::get<1>(key).contains(cityQuery, Qt::CaseInsensitive);
            if (nameMatch && cityMatch) {
                results.append(values[i]);
            }
        }

        // Отображение результатов поиска
        if (!results.getLength()) {
            QMessageBox::information(this, "Search Result", "No matches found");
            indexTable->setRowCount(0);
            return;
        }

        indexTable->setRowCount(results.getLength());
        for (int i = 0; i < results.getLength(); i++) {
            const auto& person = results[i];
            indexTable->setItem(i, 0, new QTableWidgetItem(person.name));
            indexTable->setItem(i, 1, new QTableWidgetItem(person.city));
            indexTable->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
            indexTable->setItem(i, 3, new QTableWidgetItem(QString::number(person.salary)));
        }
    }

    // Метод отображения всех записей индекса
    void showAllIndexEntries() {
        ArraySequence<std::tuple<QString, QString>> keys = compositeIndex.GetAllKeys();
        ArraySequence<Person> values = compositeIndex.GetAllValues();

        allIndexTable->setRowCount(keys.getLength());
        allIndexTable->setColumnCount(4); // Name, City, Age, Salary
        allIndexTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});

        for (int i = 0; i < keys.getLength(); ++i) {
            const auto& key = keys[i]; // std::tuple<QString, QString>
            const Person& person = values[i];
            allIndexTable->setItem(i, 0, new QTableWidgetItem(std::get<0>(key)));
            allIndexTable->setItem(i, 1, new QTableWidgetItem(std::get<1>(key)));
            allIndexTable->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
            allIndexTable->setItem(i, 3, new QTableWidgetItem(QString::number(person.salary)));
        }
    }

private slots:
    // Слот добавления нового человека
    void addPerson() {
        bool ok;
        QString name = QInputDialog::getText(this, "Add Person", "Name:", QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty()) return;

        QString city = QInputDialog::getText(this, "Add Person", "City:", QLineEdit::Normal, "", &ok);
        if (!ok || city.isEmpty()) return;

        int age = QInputDialog::getInt(this, "Add Person", "Age:", 0, 0, 120, 1, &ok);
        if (!ok) return;

        double salary = QInputDialog::getDouble(this, "Add Person", "Salary:", 0, 0, 1000000, 2, &ok);
        if (!ok) return;

        Person newPerson(name, city, age, salary);
        try {
            compositeIndex.Add(newPerson);
        } catch (const std::invalid_argument& e) {
            QMessageBox::warning(this, "Add Person", e.what());
            return;
        }
        people.append(newPerson);
        updateDataTable();
    }

    // Слот удаления выбранного человека
    void removePerson() {
        int row = dataTable->currentRow();
        if (row >= 0 && row < people.getLength()) {
            Person personToRemove = people[row];
            // Удаление из индекса
            std::tuple<QString, QString> keyToRemove(personToRemove.name, personToRemove.city);
            try {
                compositeIndex.Remove(keyToRemove);
            } catch (const std::out_of_range& e) {
                QMessageBox::warning(this, "Remove Person", e.what());
                return;
            }
            // Удаление из списка людей
            people.removeAt(row);
            updateDataTable();
        }
    }

    // Метод обновления таблицы данных
    void updateDataTable() {
        dataTable->setRowCount(people.getLength());
        for (int i = 0; i < people.getLength(); i++) {
            const auto& person = people[i];
            dataTable->setItem(i, 0, new QTableWidgetItem(person.name));
            dataTable->setItem(i, 1, new QTableWidgetItem(person.city));
            dataTable->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
            dataTable->setItem(i, 3, new QTableWidgetItem(QString::number(person.salary)));
        }
    }

    // Метод инициализации данных
    void initializeData() {
        Person p1("John", "New York", 30, 75000);
        Person p2("Alice", "Los Angeles", 25, 65000);
        Person p3("Bob", "Chicago", 45, 90000);
        Person p4("Eve", "New York", 29, 72000);
        Person p5("Charlie", "Los Angeles", 34, 80000);
        Person p6("Grace", "Chicago", 28, 67000);
        Person p7("Oscar", "New York", 41, 95000);
        Person p8("Sophia", "Los Angeles", 23, 62000);
        Person p9("Liam", "Chicago", 37, 87000);
        Person p10("Emily", "New York", 31, 78000);
        Person p11("James", "Los Angeles", 36, 84000);
        Person p12("Mia", "Chicago", 27, 69000);
        Person p13("Henry", "New York", 48, 105000);
        Person p14("Isabella", "Los Angeles", 26, 64000);
        Person p15("David", "Chicago", 33, 75000);
        Person p16("Ella", "New York", 39, 92000);
        Person p17("Ethan", "Los Angeles", 24, 61000);
        Person p18("Ava", "Chicago", 29, 120000);
        Person p19("Michael", "New York", 52, 170000);
        Person p20("Harper", "Los Angeles", 31, 76000);
        Person p21("Noah", "Chicago", 80, 88000);
        Person p22("Chloe", "New York", 27, 70000);
        Person p23("Logan", "Los Angeles", 34, 82000);
        Person p24("Amelia", "Chicago", 38, 100000);
        Person p25("Olivia", "New York", 28, 71000);
        Person p26("Lucas", "Los Angeles", 46, 97000);
        Person p27("Jack", "Chicago", 30, 74000);
        Person p28("Natalie", "New York", 32, 80000);
        Person p29("Mason", "Los Angeles", 44, 93000);
        Person p30("Zoe", "Chicago", 35, 85000);
        Person p31("Victoria", "New York", 39, 92000);
        Person p32("Owen", "Los Angeles", 26, 66000);
        Person p33("Lily", "Chicago", 41, 94000);
        Person p34("Eleanor", "New York", 99, 73000);
        Person p35("Benjamin", "Los Angeles", 38, 86000);
        Person p36("Scarlett", "Chicago", 12, 1000);
        Person p37("William", "New York", 51, 108000);
        Person p38("Sophie", "Los Angeles", 25, 65000);
        Person p39("Ellaine", "Chicago", 31, 77000);
        Person p40("Charlotte", "New York", 28, 72000);
        Person p41("Henryk", "Los Angeles", 37, 89000);
        Person p42("Alexander", "Chicago", 46, 97000);
        Person p43("Luna", "New York", 33, 80000);
        Person p44("Samuel", "Los Angeles", 30, 75000);
        Person p45("Avery", "Chicago", 44, 93000);
        Person p46("Isaac", "New York", 40, 94000);
        Person p47("Penelope", "Los Angeles", 32, 81000);
        Person p48("Hannah", "Chicago", 27, 69000);
        Person p49("Daniel", "New York", 47, 99000);
        Person p50("Stella", "Los Angeles", 22, 62000);
        Person p51("Levi", "Chicago", 35, 86000);
        Person p52("Aria", "New York", 26, 67000);
        Person p53("Sebastian", "Los Angeles", 41, 92000);
        Person p54("Madison", "Chicago", 39, 88000);
        Person p55("Gabriel", "New York", 36, 85000);
        Person p56("Camila", "Los Angeles", 28, 70000);
        Person p57("Joseph", "Chicago", 45, 96000);
        Person p58("Nora", "New York", 32, 79000);
        Person p59("Ezra", "Los Angeles", 50, 104000);
        Person p60("Layla", "Chicago", 34, 82000);



        people.append(p1);
        people.append(p2);
        people.append(p3);
        people.append(p4);
        people.append(p5);
        people.append(p6);
        people.append(p7);
        people.append(p8);
        people.append(p9);
        people.append(p10);
        people.append(p11);
        people.append(p12);
        people.append(p13);
        people.append(p14);
        people.append(p15);
        people.append(p16);
        people.append(p17);
        people.append(p18);
        people.append(p19);
        people.append(p20);
        people.append(p21);
        people.append(p22);
        people.append(p23);
        people.append(p24);
        people.append(p25);
        people.append(p26);
        people.append(p27);
        people.append(p28);
        people.append(p29);
        people.append(p30);
        people.append(p31);
        people.append(p32);
        people.append(p33);
        people.append(p34);
        people.append(p35);
        people.append(p36);
        people.append(p37);
        people.append(p38);
        people.append(p39);
        people.append(p40);
        people.append(p41);
        people.append(p42);
        people.append(p43);
        people.append(p44);
        people.append(p45);
        people.append(p46);
        people.append(p47);
        people.append(p48);
        people.append(p49);
        people.append(p50);
        people.append(p51);
        people.append(p52);
        people.append(p53);
        people.append(p54);
        people.append(p55);
        people.append(p56);
        people.append(p57);
        people.append(p58);
        people.append(p59);
        people.append(p60);


        try {
            compositeIndex.Add(p1);
            compositeIndex.Add(p2);
            compositeIndex.Add(p3);
            compositeIndex.Add(p4);
            compositeIndex.Add(p5);
            compositeIndex.Add(p6);
            compositeIndex.Add(p7);
            compositeIndex.Add(p8);
            compositeIndex.Add(p9);
            compositeIndex.Add(p10);
            compositeIndex.Add(p11);
            compositeIndex.Add(p12);
            compositeIndex.Add(p13);
            compositeIndex.Add(p14);
            compositeIndex.Add(p15);
            compositeIndex.Add(p16);
            compositeIndex.Add(p17);
            compositeIndex.Add(p18);
            compositeIndex.Add(p19);
            compositeIndex.Add(p20);
            compositeIndex.Add(p21);
            compositeIndex.Add(p22);
            compositeIndex.Add(p23);
            compositeIndex.Add(p24);
            compositeIndex.Add(p25);
            compositeIndex.Add(p26);
            compositeIndex.Add(p27);
            compositeIndex.Add(p28);
            compositeIndex.Add(p29);
            compositeIndex.Add(p30);
            compositeIndex.Add(p31);
            compositeIndex.Add(p32);
            compositeIndex.Add(p33);
            compositeIndex.Add(p34);
            compositeIndex.Add(p35);
            compositeIndex.Add(p36);
            compositeIndex.Add(p37);
            compositeIndex.Add(p38);
            compositeIndex.Add(p39);
            compositeIndex.Add(p40);
            compositeIndex.Add(p41);
            compositeIndex.Add(p42);
            compositeIndex.Add(p43);
            compositeIndex.Add(p44);
            compositeIndex.Add(p45);
            compositeIndex.Add(p46);
            compositeIndex.Add(p47);
            compositeIndex.Add(p48);
            compositeIndex.Add(p49);
            compositeIndex.Add(p50);
            compositeIndex.Add(p51);
            compositeIndex.Add(p52);
            compositeIndex.Add(p53);
            compositeIndex.Add(p54);
            compositeIndex.Add(p55);
            compositeIndex.Add(p56);
            compositeIndex.Add(p57);
            compositeIndex.Add(p58);
            compositeIndex.Add(p59);
            compositeIndex.Add(p60);


        } catch (const std::invalid_argument& e) {
            QMessageBox::warning(this, "Initialize Data", e.what());
        }

        updateDataTable();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

