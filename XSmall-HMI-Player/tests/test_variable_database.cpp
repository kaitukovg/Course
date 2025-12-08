#include <gtest/gtest.h>
#include "VariableDatabase.h"
#include <VisualObject.h>

TEST(VariableDatabaseTest, SetAndGetVariable) {
    VariableDatabase db;
    
    db.setVariable("test_var", 42.5);
    EXPECT_DOUBLE_EQ(db.getVariable("test_var"), 42.5);
}

TEST(VariableDatabaseTest, VariableExists) {
    VariableDatabase db;
    
    EXPECT_FALSE(db.variableExists("nonexistent"));
    db.setVariable("test_var", 10.0);
    EXPECT_TRUE(db.variableExists("test_var"));
}

TEST(VariableDatabaseTest, HistoryStorage) {
    VariableDatabase db;
    
    // Проверяем, что история сохраняется
    db.setVariable("history_var", 1.0);
    db.setVariable("history_var", 2.0);
    db.setVariable("history_var", 3.0);
    
    const auto& history = db.getHistory("history_var");
    EXPECT_GE(history.size(), 3);
}

TEST(VariableDatabaseTest, Subscription) {
    VariableDatabase db;
    double callbackValue = 0.0;
    
    // Тестируем систему подписок
    db.subscribe("sub_var", [&callbackValue](double value) {
        callbackValue = value;
    });
    
    db.setVariable("sub_var", 99.9);
    EXPECT_DOUBLE_EQ(callbackValue, 99.9);
}
