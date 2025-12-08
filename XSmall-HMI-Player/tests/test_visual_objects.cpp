#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "VariableDatabase.h"
#include "VisualObject.h"  
#include "Rectangle.h"
#include "Text.h"
#include "SceneFactory.h"

class VisualObjectsTest : public ::testing::Test {
protected:
    VariableDatabase db;
    sf::Font font;
    
    void SetUp() override {
        // Пробуем несколько путей к шрифту
        std::vector<std::string> fontPaths = {
            "assets/fonts/helveticabold.ttf",
            "../assets/fonts/helveticabold.ttf",
            "../../assets/fonts/helveticabold.ttf",
            "C:/Windows/Fonts/arial.ttf"
        };
        
        bool loaded = false;
        for (const auto& path : fontPaths) {
            if (font.loadFromFile(path)) {
                std::cout << "Test font is loaded from: " << path << std::endl;
                loaded = true;
                break;
            }
        }
        
        if (!loaded) {
            // Если шрифт не загрузился, не прерываем тесты полностью
            std::cerr << "WARNING: cannot load text font!" << std::endl;
            // Продолжаем тесты, но некоторые могут упасть
        }
    }
};

TEST_F(VisualObjectsTest, RectangleCreation) {
    Rectangle rect(10.0f, 20.0f, 100.0f, 50.0f, 
                   sf::Color::Red, "TestRect", &db);
    EXPECT_EQ(rect.getName(), "TestRect");
}

TEST_F(VisualObjectsTest, TextCreation) {
    // Проверяем, что шрифт загружен
    if (font.getInfo().family.empty()) {
        GTEST_SKIP() << "Font cannot be load, skip the test TextCreation";
    }
    
    Text text(10.0f, 20.0f, "Hello World", 
              &font, 16, sf::Color::Black, "TestText", &db);
    EXPECT_EQ(text.getName(), "TestText");
}

TEST_F(VisualObjectsTest, RectangleWithVariable) {
    // Тестируем прямоугольник с привязанной переменной
    Rectangle rect(10.0f, 20.0f, 100.0f, 50.0f, 
                   sf::Color::Blue, "VarRect", &db, "test_var");
    rect.addCondition(1.0, sf::Color::Green);
    
    db.setVariable("test_var", 1.0);
    EXPECT_DOUBLE_EQ(db.getVariable("test_var"), 1.0);
}

TEST_F(VisualObjectsTest, XmlLoaderCreatesObjects) {
    // Проверяем, что шрифт загружен
    if (font.getInfo().family.empty()) {
        GTEST_SKIP() << "Font cannot be load, skip the test XmlLoaderCreatesObjects";
    }
    
    auto objects = SceneFactory::createDemoScene(&db, &font);
    EXPECT_GT(objects.size(), 0) << "There must be at least one object";
    
    bool hasRectangle = false;
    bool hasText = false;
    bool hasButton = false;
    
    for (const auto& obj : objects) {
        std::string name = obj->getName();
        if (name.find("Rectangle") != std::string::npos ||
            name.find("Panel") != std::string::npos) hasRectangle = true;
        if (name.find("Text") != std::string::npos) hasText = true;
        if (name.find("Button") != std::string::npos) hasButton = true;
    }
    
    EXPECT_TRUE(hasRectangle);
    EXPECT_TRUE(hasText);
    EXPECT_TRUE(hasButton);
}
