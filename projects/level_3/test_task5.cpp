#include "task5.h"
#include <QFile>
#include <gtest/gtest.h>

class TempFile {
public:
  QString filename;
  TempFile(const QString &fname = "test_accounts.dat") : filename(fname) {
    QFile::remove(filename);
  }
  ~TempFile() { QFile::remove(filename); }
};

TEST(AccountManagerTest, AddAndFindAccount) {
  TempFile tmp("test_accounts.dat");
  AccountManager mgr("test_accounts.dat");
  ASSERT_TRUE(mgr.load());

  Account acc;
  strcpy(acc.cardNumber, "1111222233334444");
  QString pinHash = PinCrypto::encrypt("1111");
  strcpy(acc.pinHash, pinHash.toLatin1().constData());
  acc.balance = 500.0;
  acc.active = true;

  EXPECT_TRUE(mgr.addAccount(acc));
  EXPECT_EQ(mgr.count(), 1);

  int idx = mgr.findAccount("1111222233334444");
  EXPECT_GE(idx, 0);
  Account readAcc = mgr.readAccount(idx);
  EXPECT_EQ(readAcc.balance, 500.0);
  EXPECT_TRUE(readAcc.active);
}

TEST(AccountManagerTest, UpdateAccount) {
  TempFile tmp("test_accounts.dat");
  AccountManager mgr("test_accounts.dat");
  mgr.load();

  Account acc;
  strcpy(acc.cardNumber, "1111222233334444");
  QString pinHash = PinCrypto::encrypt("1111");
  strcpy(acc.pinHash, pinHash.toLatin1().constData());
  acc.balance = 500.0;
  acc.active = true;
  mgr.addAccount(acc);

  int idx = mgr.findAccount("1111222233334444");
  Account readAcc = mgr.readAccount(idx);
  readAcc.balance = 1000.0;
  EXPECT_TRUE(mgr.writeAccount(idx, readAcc));

  Account updated = mgr.readAccount(idx);
  EXPECT_EQ(updated.balance, 1000.0);
}

TEST(AccountManagerTest, CardExists) {
  TempFile tmp("test_accounts.dat");
  AccountManager mgr("test_accounts.dat");
  mgr.load();

  EXPECT_FALSE(mgr.isCardExists("0000000000000000"));

  Account acc;
  strcpy(acc.cardNumber, "1111222233334444");
  QString pinHash = PinCrypto::encrypt("1111");
  strcpy(acc.pinHash, pinHash.toLatin1().constData());
  mgr.addAccount(acc);

  EXPECT_TRUE(mgr.isCardExists("1111222233334444"));
}

TEST(PinCryptoTest, EncryptVerify) {
  QString pin = "1234";
  QString hash = PinCrypto::encrypt(pin);
  EXPECT_NE(hash, pin);
  EXPECT_TRUE(PinCrypto::verify(pin, hash));
  EXPECT_FALSE(PinCrypto::verify("4321", hash));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}