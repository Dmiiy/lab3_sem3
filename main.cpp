// main.cpp
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
#include <limits>
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
        resize(1300, 900);

        // Central Widget and Layout
        auto* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        auto* mainLayout = new QVBoxLayout(centralWidget);

        // Tab Widget
        auto* tabWidget = new QTabWidget(this);
        mainLayout->addWidget(tabWidget);

        // Create Tabs
        createDataTab(tabWidget);
        createHistogramTab(tabWidget);
        createIndexTab(tabWidget);

        // Initialize Data
        initializeData();
    }

private:
    // Widgets
    QTableWidget* dataTable;         // Data Table
    QChartView* histogramView;       // Histogram Chart
    QTableWidget* statsTable;        // Class Statistics Table
    QTableWidget* indexTable;        // Search Results Table
    QTableWidget* allIndexTable;     // All Index Entries Table

    // Data Structures
    ArraySequence<Person> people;    // Sequence of People
    Index<Person, QString, QString> compositeIndex; // Composite Index

    // Create Data Tab
    void createDataTab(QTabWidget* tabWidget) {
        auto* dataWidget = new QWidget();
        auto* layout = new QVBoxLayout(dataWidget);

        // Data Table Setup
        dataTable = new QTableWidget();
        dataTable->setColumnCount(4);
        dataTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});
        dataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        dataTable->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(dataTable);

        // Buttons Layout
        auto* buttonLayout = new QHBoxLayout();
        auto* addButton = new QPushButton("Add Person");
        auto* removeButton = new QPushButton("Remove Person");
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addStretch();
        layout->addLayout(buttonLayout);

        // Connect Signals and Slots
        connect(addButton, &QPushButton::clicked, this, &MainWindow::addPerson);
        connect(removeButton, &QPushButton::clicked, this, &MainWindow::removePersonByName);

        // Add Tab
        tabWidget->addTab(dataWidget, "Data");
    }

    // Create Histogram Tab
    void createHistogramTab(QTabWidget* tabWidget) {
        auto* histogramWidget = new QWidget();
        auto* layout = new QVBoxLayout(histogramWidget);

        // Controls Layout
        auto* controlLayout = new QHBoxLayout();
        auto* criteriaLabel = new QLabel("Criteria:");
        auto* criteriaCombo = new QComboBox();
        criteriaCombo->addItems({"Age", "Salary"});
        auto* generateButton = new QPushButton("Generate Histogram");
        controlLayout->addWidget(criteriaLabel);
        controlLayout->addWidget(criteriaCombo);
        controlLayout->addWidget(generateButton);
        controlLayout->addStretch();
        layout->addLayout(controlLayout);

        // Histogram View
        histogramView = new QChartView();
        histogramView->setRenderHint(QPainter::Antialiasing);
        layout->addWidget(histogramView, 3);

        // Statistics Table
        statsTable = new QTableWidget();
        statsTable->setColumnCount(5);
        statsTable->setHorizontalHeaderLabels({"Class", "Count", "Sum", "Min", "Max"});
        statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        statsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        statsTable->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(new QLabel("Class Statistics:"));
        layout->addWidget(statsTable, 1);

        // Connect Signals and Slots
        connect(generateButton, &QPushButton::clicked, [this, criteriaCombo]() {
            generateHistogram(criteriaCombo->currentText());
        });

        // Add Tab
        tabWidget->addTab(histogramWidget, "Histogram");
    }

    // Create Index Tab
    void createIndexTab(QTabWidget* tabWidget) {
        auto* indexWidget = new QWidget();
        auto* layout = new QVBoxLayout(indexWidget);

        // Search Group Layout
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
        searchGroupLayout->addStretch();
        layout->addLayout(searchGroupLayout);

        // Range Search Layout
        auto* rangeLayout = new QHBoxLayout();
        auto* ageRangeLabel = new QLabel("Age Range:");
        auto* ageStart = new QLineEdit();
        ageStart->setPlaceholderText("Min");
        auto* ageEnd = new QLineEdit();
        ageEnd->setPlaceholderText("Max");
        auto* salaryRangeLabel = new QLabel("Salary Range:");
        auto* salaryStart = new QLineEdit();
        salaryStart->setPlaceholderText("Min");
        auto* salaryEnd = new QLineEdit();
        salaryEnd->setPlaceholderText("Max");
        auto* rangeSearchButton = new QPushButton("Range Search");
        rangeLayout->addWidget(ageRangeLabel);
        rangeLayout->addWidget(ageStart);
        rangeLayout->addWidget(ageEnd);
        rangeLayout->addWidget(salaryRangeLabel);
        rangeLayout->addWidget(salaryStart);
        rangeLayout->addWidget(salaryEnd);
        rangeLayout->addWidget(rangeSearchButton);
        rangeLayout->addStretch();
        layout->addLayout(rangeLayout);

        // Search Results Table
        indexTable = new QTableWidget();
        indexTable->setColumnCount(4);
        indexTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});
        indexTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        indexTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        indexTable->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(new QLabel("Search Results:"));
        layout->addWidget(indexTable, 2);

        // Show All Index Entries Button
        auto* showAllButton = new QPushButton("Show All Index Entries");
        layout->addWidget(showAllButton);

        // All Index Entries Table
        allIndexTable = new QTableWidget();
        allIndexTable->setColumnCount(4);
        allIndexTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});
        allIndexTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        allIndexTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        allIndexTable->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(new QLabel("All Index Entries:"));
        layout->addWidget(allIndexTable, 2);

        // Connect Signals and Slots
        connect(searchButton, &QPushButton::clicked, [this, nameField, cityField]() {
            QString nameQuery = nameField->text();
            QString cityQuery = cityField->text();
            searchIndex(nameQuery, cityQuery);
        });

        connect(rangeSearchButton, &QPushButton::clicked, [this, ageStart, ageEnd, salaryStart, salaryEnd]() {
            bool ok1, ok2, ok3, ok4;
            int ageMin = ageStart->text().toInt(&ok1);
            int ageMax = ageEnd->text().toInt(&ok2);
            double salaryMin = salaryStart->text().toDouble(&ok3);
            double salaryMax = salaryEnd->text().toDouble(&ok4);
            if (ok1 && ok2 && ok3 && ok4 && ageMin <= ageMax && salaryMin <= salaryMax) {
                rangeSearch(ageMin, ageMax, salaryMin, salaryMax);
            }
            else {
                QMessageBox::warning(this, "Range Search", "Invalid or inconsistent range inputs.");
            }
        });

        connect(showAllButton, &QPushButton::clicked, this, &MainWindow::showAllIndexEntries);

        // Add Tab
        tabWidget->addTab(indexWidget, "Index");
    }

    // Generate Histogram
    void generateHistogram(const QString& criteria) {
        ArraySequence<std::pair<double, double>> ranges;
        if (criteria == "Age") {
            ranges.append({ 0, 20 });
            ranges.append({ 20, 40 });
            ranges.append({ 40, 60 });
            ranges.append({ 60, 100 });
        }
        else { // Salary
            ranges.append({ 0, 50000 });
            ranges.append({ 50000, 100000 });
            ranges.append({ 100000, 150000 });
            ranges.append({ 150000, 200000 });
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

        updateHistogramChart(histogram, criteria);
    }

    // Update Histogram Chart and Statistics Table
    void updateHistogramChart(const Histogram<double, QString, Person>& histogram, const QString& criteria) {
        auto* series = new QBarSeries();
        const auto& histData = histogram.getHistogram();

        QStringList classes;
        ArraySequence<QString> categories;
        ArraySequence<std::pair<double, double>> ranges = histData.GetKeys();
        for (int i = 0; i < ranges.getLength(); i++) {
            ArraySequence<QString> classKeys = histData.GetReference(ranges[i]).GetKeys();
            for (int j = 0; j < classKeys.getLength(); j++) {
                const QString& cls = classKeys[j];
                if (!classes.contains(cls)) classes << cls;
            }
        }

        for (const QString& cls : classes) {
            auto* set = new QBarSet(cls);
            for (int i = 0; i < ranges.getLength(); i++) {
                const auto& range = ranges[i];
                const auto& classData = histData.GetReference(range);
                if (classData.ContainsKey(cls)) {
                    *set << classData.GetReference(cls).count;
                }
                else {
                    *set << 0;
                }
            }
            series->append(set);
        }

        QChart* chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(QString("Distribution by %1 and Class").arg(criteria));
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // X Axis
        auto* axisX = new QBarCategoryAxis();
        QStringList rangeLabels;
        for (int i = 0; i < ranges.getLength(); i++) {
            rangeLabels << QString("%1-%2").arg(ranges[i].first).arg(ranges[i].second);

        }
        axisX->append(rangeLabels);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Y Axis - Dynamic Range
        auto* axisY = new QValueAxis();
        int maxCount = 0;
        for (int i = 0; i < series->count(); ++i) {
            QBarSet* set = series->barSets().at(i);
            for (int j = 0; j < set->count(); ++j) { // Исправленный цикл
                qreal val = set->at(j); // Используем at() вместо values()
                if (val > maxCount) maxCount = static_cast<int>(val);
            }
        }
        axisY->setRange(0, maxCount + 5);
        axisY->setTitleText("Count");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Set Chart to View
        histogramView->setChart(chart);

        // Update Statistics Table
        updateStatsTable(histogram, classes);
    }

    // Update Statistics Table
    void updateStatsTable(const Histogram<double, QString, Person>& histogram, const QStringList& classes) {
        statsTable->setRowCount(classes.size());

        for (int i = 0; i < classes.size(); ++i) {
            const QString& cls = classes[i];
            int totalCount = 0;
            double totalSum = 0.0;
            double overallMin = std::numeric_limits<double>::max();
            double overallMax = std::numeric_limits<double>::lowest();

            ArraySequence<std::pair<double, double>> ranges = histogram.getHistogram().GetKeys();
            for (int j = 0; j < ranges.getLength(); ++j) {
                const auto& range = ranges[j];
                const auto& classData = histogram.getHistogram().GetReference(range);
                if (classData.ContainsKey(cls)) {
                    const auto& stats = classData.GetReference(cls);
                    totalCount += stats.count;
                    totalSum += stats.sum;
                    if (stats.min && *stats.min < overallMin) overallMin = *stats.min;
                    if (stats.max && *stats.max > overallMax) overallMax = *stats.max;
                }
            }

            statsTable->setItem(i, 0, new QTableWidgetItem(cls));
            statsTable->setItem(i, 1, new QTableWidgetItem(QString::number(totalCount)));
            statsTable->setItem(i, 2, new QTableWidgetItem(QString::number(totalSum, 'f', 2)));
            if (totalCount > 0) {
                statsTable->setItem(i, 3, new QTableWidgetItem(QString::number(overallMin, 'f', 2)));
                statsTable->setItem(i, 4, new QTableWidgetItem(QString::number(overallMax, 'f', 2)));
            }
            else {
                statsTable->setItem(i, 3, new QTableWidgetItem("N/A"));
                statsTable->setItem(i, 4, new QTableWidgetItem("N/A"));
            }
        }
    }

    // Search in Index
    void searchIndex(const QString& nameQuery, const QString& cityQuery) {
        ArraySequence<std::tuple<QString, QString>> keys = compositeIndex.GetAllKeys();
        ArraySequence<Person> values = compositeIndex.GetAllValues();
        ArraySequence<Person> results;

        for (int i = 0; i < keys.getLength(); ++i) {
            const auto& key = keys[i];
            bool nameMatch = nameQuery.isEmpty() || std::get<0>(key).contains(nameQuery, Qt::CaseInsensitive);
            bool cityMatch = cityQuery.isEmpty() || std::get<1>(key).contains(cityQuery, Qt::CaseInsensitive);
            if (nameMatch && cityMatch) {
                results.append(values[i]);
            }
        }

        // Display Results
        if (results.getLength() == 0) {
            QMessageBox::information(this, "Search Result", "No matches found.");
            indexTable->setRowCount(0);
            return;
        }

        indexTable->setRowCount(results.getLength());
        for (int i = 0; i < results.getLength(); ++i) {
            const auto& person = results[i];
            indexTable->setItem(i, 0, new QTableWidgetItem(person.name));
            indexTable->setItem(i, 1, new QTableWidgetItem(person.city));
            indexTable->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
            indexTable->setItem(i, 3, new QTableWidgetItem(QString::number(person.salary, 'f', 2)));
        }
    }

    // Range Search
    void rangeSearch(int ageMin, int ageMax, double salaryMin, double salaryMax) {
        ArraySequence<Person> results;

        for (int i = 0; i < people.getLength(); ++i) {
            const Person& p = people[i];
            if (p.age >= ageMin && p.age <= ageMax &&
                p.salary >= salaryMin && p.salary <= salaryMax) {
                results.append(p);
            }
        }

        // Display Results
        if (results.getLength() == 0) {
            QMessageBox::information(this, "Range Search Result", "No matches found.");
            indexTable->setRowCount(0);
            return;
        }

        indexTable->setRowCount(results.getLength());
        for (int i = 0; i < results.getLength(); ++i) {
            const auto& person = results[i];
            indexTable->setItem(i, 0, new QTableWidgetItem(person.name));
            indexTable->setItem(i, 1, new QTableWidgetItem(person.city));
            indexTable->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
            indexTable->setItem(i, 3, new QTableWidgetItem(QString::number(person.salary, 'f', 2)));
        }
    }

    // Show All Index Entries
    void showAllIndexEntries() {
        ArraySequence<std::tuple<QString, QString>> keys = compositeIndex.GetAllKeys();
        ArraySequence<Person> values = compositeIndex.GetAllValues();

        allIndexTable->setRowCount(keys.getLength());
        allIndexTable->setColumnCount(4);
        allIndexTable->setHorizontalHeaderLabels({"Name", "City", "Age", "Salary"});

        for (int i = 0; i < keys.getLength(); ++i) {
            const auto& key = keys[i];
            const Person& person = values[i];
            allIndexTable->setItem(i, 0, new QTableWidgetItem(std::get<0>(key)));
            allIndexTable->setItem(i, 1, new QTableWidgetItem(std::get<1>(key)));
            allIndexTable->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
            allIndexTable->setItem(i, 3, new QTableWidgetItem(QString::number(person.salary, 'f', 2)));
        }
    }

private slots:
    // Add Person Slot
    void addPerson() {
        bool ok;
        QString name = QInputDialog::getText(this, "Add Person", "Name:", QLineEdit::Normal, "", &ok);
        if (!ok || name.trimmed().isEmpty()) return;

        QString city = QInputDialog::getText(this, "Add Person", "City:", QLineEdit::Normal, "", &ok);
        if (!ok || city.trimmed().isEmpty()) return;

        int age = QInputDialog::getInt(this, "Add Person", "Age:", 0, 0, 120, 1, &ok);
        if (!ok) return;

        double salary = QInputDialog::getDouble(this, "Add Person", "Salary:", 0, 0, 1000000, 2, &ok);
        if (!ok) return;

        Person newPerson(name.trimmed(), city.trimmed(), age, salary);
        try {
            compositeIndex.Add(newPerson);
            people.append(newPerson);
            updateDataTable();
            QMessageBox::information(this, "Add Person", "Person added successfully.");
        }
        catch (const std::invalid_argument& e) {
            QMessageBox::warning(this, "Add Person", e.what());
        }
    }

    // Remove Person Slot
    void removePersonByName() {
        bool ok;
        QString nameToRemove = QInputDialog::getText(this, "Remove Person", "Enter Name to Remove:", QLineEdit::Normal, "", &ok);
        if (!ok || nameToRemove.trimmed().isEmpty()) return;

        ArraySequence<std::tuple<QString, QString>> keys = compositeIndex.GetAllKeys();
        ArraySequence<Person> values = compositeIndex.GetAllValues();
        std::vector<int> matchingIndices;

        for (int i = 0; i < keys.getLength(); ++i) {
            if (std::get<0>(keys[i]).compare(nameToRemove, Qt::CaseInsensitive) == 0) {
                matchingIndices.push_back(i);
            }
        }

        if (matchingIndices.empty()) {
            QMessageBox::information(this, "Remove Person", "Person not found.");
            return;
        }

        // If multiple matches, ask user to select
        if (matchingIndices.size() > 1) {
            QStringList choices;
            for (const auto& idx : matchingIndices) {
                const Person& p = values[idx];
                choices << QString("Name: %1, City: %2, Age: %3, Salary: %4")
                        .arg(p.name)
                        .arg(p.city)
                        .arg(p.age)
                        .arg(p.salary, 0, 'f', 2);
            }
            bool okSelection;
            QString selected = QInputDialog::getItem(this, "Remove Person", "Multiple entries found. Select one to remove:", choices, 0, false, &okSelection);
            if (!okSelection || selected.isEmpty()) return;

            // Find selected index
            int selectedIndex = -1;
            for (int i = 0; i < matchingIndices.size(); ++i) {
                const Person& p = values[matchingIndices[i]];
                QString entry = QString("Name: %1, City: %2, Age: %3, Salary: %4")
                        .arg(p.name)
                        .arg(p.city)
                        .arg(p.age)
                        .arg(p.salary, 0, 'f', 2);
                if (entry == selected) {
                    selectedIndex = matchingIndices[i];
                    break;
                }
            }

            if (selectedIndex != -1) {
                try {
                    compositeIndex.Remove(keys[selectedIndex]);
                    for (int i = 0; i < people.getLength(); ++i) {
                        if (people[i].age==values[selectedIndex].age && people[i].salary==values[selectedIndex].salary && people[i].city==values[selectedIndex].city && people[i].name==values[selectedIndex].name) {
                            people.removeAt(i);
                            break;
                        }
                    }
                    //people.removeAt(selectedIndex);
                    updateDataTable();
                    QMessageBox::information(this, "Remove Person", "Person removed successfully.");
                }
                catch (const std::out_of_range& e) {
                    QMessageBox::warning(this, "Remove Person", e.what());
                }
            }
        }
        else {
            // Single match
            int indexToRemove = matchingIndices[0];
            try {
                compositeIndex.Remove(keys[indexToRemove]);
                for (int i = 0; i < people.getLength(); ++i) {
                    if (people[i].age==values[indexToRemove].age && people[i].salary==values[indexToRemove].salary && people[i].city==values[indexToRemove].city && people[i].name==values[indexToRemove].name) {
                        people.removeAt(i);
                        break;
                    }
                }
                updateDataTable();
                QMessageBox::information(this, "Remove Person", "Person removed successfully.");
            }
            catch (const std::out_of_range& e) {
                QMessageBox::warning(this, "Remove Person", e.what());
            }
        }
    }

    // Update Data Table
    void updateDataTable() {
        dataTable->setRowCount(people.getLength());
        for (int i = 0; i < people.getLength(); i++) {
            const auto& person = people[i];
            dataTable->setItem(i, 0, new QTableWidgetItem(person.name));
            dataTable->setItem(i, 1, new QTableWidgetItem(person.city));
            dataTable->setItem(i, 2, new QTableWidgetItem(QString::number(person.age)));
            dataTable->setItem(i, 3, new QTableWidgetItem(QString::number(person.salary, 'f', 2)));
        }
    }

    // Initialize Data
    void initializeData() {
        // Sample Data
        Person persons[] = {
                Person("John", "New York", 30, 75000),
                Person("Alice", "Los Angeles", 25, 65000),
                Person("Bob", "Chicago", 45, 90000),
                Person("Eve", "New York", 29, 72000),
                Person("Charlie", "Los Angeles", 34, 80000),
                Person("Grace", "Chicago", 28, 67000),
                Person("Oscar", "New York", 41, 95000),
                Person("Sophia", "Los Angeles", 23, 62000),
                Person("Liam", "Chicago", 37, 87000),
                Person("Emily", "New York", 31, 78000),
                Person("James", "Los Angeles", 36, 84000),
                Person("Mia", "Chicago", 27, 69000),
                Person("Henry", "New York", 48, 105000),
                Person("Isabella", "Los Angeles", 26, 64000),
                Person("David", "Chicago", 33, 75000),
                Person("Ella", "New York", 39, 92000),
                Person("Ethan", "Los Angeles", 24, 61000),
                Person("Ava", "Chicago", 29, 120000),
                Person("Michael", "New York", 52, 170000),
                Person("Harper", "Los Angeles", 31, 76000),
                Person("Noah", "Chicago", 80, 88000),
                Person("Chloe", "New York", 27, 70000),
                Person("Logan", "Los Angeles", 34, 82000),
                Person("Amelia", "Chicago", 38, 100000),
                Person("Olivia", "New York", 28, 71000),
                Person("Lucas", "Los Angeles", 46, 97000),
                Person("Jack", "Chicago", 30, 74000),
                Person("Natalie", "New York", 32, 80000),
                Person("Mason", "Los Angeles", 44, 93000),
                Person("Zoe", "Chicago", 35, 85000),
                Person("Victoria", "New York", 39, 92000),
                Person("Owen", "Los Angeles", 26, 66000),
                Person("Lily", "Chicago", 41, 94000),
                Person("Eleanor", "New York", 99, 73000),
                Person("Benjamin", "Los Angeles", 38, 86000),
                Person("Scarlett", "Chicago", 12, 1000),
                Person("William", "New York", 51, 108000),
                Person("Sophie", "Los Angeles", 25, 65000),
                Person("Ellaine", "Chicago", 31, 77000),
                Person("Charlotte", "New York", 28, 72000),
                Person("Henryk", "Los Angeles", 37, 89000),
                Person("Alexander", "Chicago", 46, 97000),
                Person("Luna", "New York", 33, 80000),
                Person("Samuel", "Los Angeles", 30, 75000),
                Person("Avery", "Chicago", 44, 93000),
                Person("Isaac", "New York", 40, 94000),
                Person("Penelope", "Los Angeles", 32, 81000),
                Person("Hannah", "Chicago", 27, 69000),
                Person("Daniel", "New York", 47, 99000),
                Person("Stella", "Los Angeles", 22, 62000),
                Person("Levi", "Chicago", 35, 86000),
                Person("Aria", "New York", 26, 67000),
                Person("Sebastian", "Los Angeles", 41, 92000),
                Person("Madison", "Chicago", 39, 88000),
                Person("Gabriel", "New York", 36, 85000),
                Person("Camila", "Los Angeles", 28, 70000),
                Person("Joseph", "Chicago", 45, 96000),
                Person("Nora", "New York", 32, 79000),
                Person("Ezra", "Los Angeles", 50, 104000),
                Person("Layla", "Chicago", 34, 82000)
        };

        // Add Persons to Sequence and Index
        for (const auto& person : persons) {
            try {
                compositeIndex.Add(person);
                people.append(person);
            }
            catch (const std::invalid_argument& e) {
                QMessageBox::warning(this, "Initialize Data", e.what());
            }
        }

        // Update Data Table
        updateDataTable();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}