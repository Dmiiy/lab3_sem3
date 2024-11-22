//#ifndef USER_H
//#define USER_H
//
//#include <QString>
//#include <QRandomGenerator>
//
//class User {
//private:
//    QString firstName;
//    QString lastName;
//    int age;
//
//public:
//    User() = default;
//
//    User(const QString &firstName, const QString &lastName, int age)
//            : firstName(firstName), lastName(lastName), age(age) {}
//
//    QString getFirstName() const { return firstName; }
//    void setFirstName(const QString &name) { firstName = name; }
//
//    QString getLastName() const { return lastName; }
//    void setLastName(const QString &surname) { lastName = surname; }
//
//    int getAge() const { return age; }
//    void setAge(int newAge) { age = newAge; }
//
//    bool operator==(const User &other) const {
//        return firstName == other.firstName && lastName == other.lastName && age == other.age;
//    }
//
//    bool operator!=(const User &other) const {
//        return !(*this == other);
//    }
//
//    static User generateRandomUser() {
//        QString firstName = generateRandomString(10);
//        QString lastName = generateRandomString(10);
//        int age = QRandomGenerator::global()->bounded(0, 101);
//        return User(firstName, lastName, age);
//    }
//
//    static QString generateRandomString(int length) {
//        const QString characters("abcdefghijklmnopqrstuvwxyz");
//        QString result;
//        for (int i = 0; i < length; ++i)
//            result.append(characters[QRandomGenerator::global()->bounded(characters.size())]);
//        return result;
//    }
//};
//
//#endif // USER_H
