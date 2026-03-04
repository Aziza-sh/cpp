#include "task4.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <gtest/gtest.h>

class TempFile {
public:
  QString name;
  TempFile() {
    name = "test_contacts.txt";
    QFile::remove(name);
  }
  ~TempFile() { QFile::remove(name); }
};

TEST(NotebookServiceTest, AddAndLoad) {
  TempFile tmp;
  {
    NotebookService service(tmp.name);
    service.addContact({"Иван", "123", "ivan@mail.ru"});
    service.addContact({"Петр", "456", "petr@mail.ru"});
    EXPECT_EQ(service.count(), 2);
  }
  NotebookService service2(tmp.name);
  EXPECT_EQ(service2.count(), 2);
  const Contact &c = service2.getContact(0);
  EXPECT_EQ(c.name, "Иван");
  EXPECT_EQ(c.phone, "123");
  EXPECT_EQ(c.email, "ivan@mail.ru");
}

TEST(NotebookServiceTest, DeleteContact) {
  TempFile tmp;
  NotebookService service(tmp.name);
  service.addContact({"Иван", "123", "ivan@mail.ru"});
  service.addContact({"Петр", "456", "petr@mail.ru"});
  EXPECT_EQ(service.count(), 2);
  service.deleteContact(0);
  EXPECT_EQ(service.count(), 1);
  const Contact &c = service.getContact(0);
  EXPECT_EQ(c.name, "Петр");
}

TEST(NotebookServiceTest, UpdateContact) {
  TempFile tmp;
  NotebookService service(tmp.name);
  service.addContact({"Иван", "123", "ivan@mail.ru"});
  service.updateContact(0, {"Иван Петров", "999", "ivan@new.ru"});
  const Contact &c = service.getContact(0);
  EXPECT_EQ(c.name, "Иван Петров");
  EXPECT_EQ(c.phone, "999");
  EXPECT_EQ(c.email, "ivan@new.ru");
}

TEST(NotebookServiceTest, SearchByName) {
  TempFile tmp;
  NotebookService service(tmp.name);
  service.addContact({"Иван", "123", "ivan@mail.ru"});
  service.addContact({"Петр", "456", "petr@mail.ru"});
  service.addContact({"Иван Петров", "789", "ivan.p@mail.ru"});
  auto results = service.searchByName("иван");
  EXPECT_EQ(results.size(), 2);
  bool found = false;
  for (const auto &c : results) {
    if (c.name == "Иван")
      found = true;
  }
  EXPECT_TRUE(found);
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}