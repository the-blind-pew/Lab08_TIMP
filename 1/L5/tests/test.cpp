#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Account.h>
#include <Transaction.h>


TEST(Account, Exceptions)
{
	Account a1(1, 1000);
    EXPECT_ANY_THROW(a1.ChangeBalance(1));
	a1.Lock();
    EXPECT_ANY_THROW(a1.Lock());
    a1.ChangeBalance(1);
	a1.Unlock();
	int balance = a1.GetBalance();
	EXPECT_EQ(balance, 1001);
}

class MockAccount: public Account {
public:
    MockAccount(int id, int balance)
        : Account(id, balance) {}
    MOCK_METHOD0(GetBalance, int());
    MOCK_METHOD1(ChangeBalance, void(int));
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};

TEST(Account, MockAcc)
{
	MockAccount account1(5, 7000);
	EXPECT_CALL(account1, ChangeBalance(1)).Times(1);
    EXPECT_CALL(account1, Lock()).Times(1);
    EXPECT_CALL(account1, Unlock()).Times(1);
    EXPECT_CALL(account1, GetBalance()).Times(1).WillOnce(testing::Return(7000));
    account1.Lock();
    account1.ChangeBalance(1);
    account1.Unlock();
    account1.GetBalance();
                
}

TEST(Transaction, WithMock) {
    MockAccount account1(5, 7000);
    MockAccount account2(6, 20000);
    
//    EXPECT_CALL(account1, ChangeBalance(2000)).Times(1);
    EXPECT_CALL(account1, Lock()).Times(1);
    EXPECT_CALL(account1, Unlock()).Times(1);
//    EXPECT_CALL(account1, GetBalance()).Times(1);
    
//    EXPECT_CALL(account2, ChangeBalance(2000)).Times(testing::AnyNumber());
//    EXPECT_CALL(account2, Lock()).Times(1);
//    EXPECT_CALL(account2, Unlock()).Times(1);
//    EXPECT_CALL(account2, GetBalance()).Times(1);
    
    Transaction tr;
    tr.set_fee(100);
    EXPECT_EQ(tr.fee(), 100);
    tr.Make(account1, account2, 2000);
    //tr.SaveToDataBase(account1, account2, 2000);
}

TEST(Transaction, Exceptions) {
    Account account1(5, 7000);
    Account account2(6, 20000);
    Transaction tr;
    Transaction tr1;
    Account account11(5, 7000);
    Account account21(6, 20000);
    tr.set_fee(10);
    EXPECT_ANY_THROW(tr.Make(account1, account1, 500));
    EXPECT_ANY_THROW(tr.Make(account1, account2, 90));
    EXPECT_ANY_THROW(tr.Make(account1, account2, 10));
    EXPECT_ANY_THROW(tr.Make(account1, account2, -7100));
    tr1.set_fee(400);
    tr1.Make(account11, account21, 8000);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
