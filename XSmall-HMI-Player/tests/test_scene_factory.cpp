#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "SceneFactory.h"
#include "VariableDatabase.h"
#include "VisualObject.h"  

class SceneFactoryTest : public ::testing::Test {
protected:
    VariableDatabase db;
    sf::Font font;
    
    void SetUp() override {
        // Пробуем несколько путей к шрифту
        std::vector<std::string> fontPaths = {            
            "../assets/fonts/helveticabold.ttf",
            "assets/fonts/helveticabold.ttf",
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
            std::cerr << "WARNING: couldn't load test font!" << std::endl;
        }
    }
};

TEST_F(SceneFactoryTest, CreateDemoScene) {
    // Проверяем, что шрифт загружен
    if (font.getInfo().family.empty()) {
        GTEST_SKIP() << "The font is not loaded, we skip the test CreateDemoScene";
    }
    
    auto objects = SceneFactory::createDemoScene(&db, &font);
    
    // Проверяем, что объекты созданы
    ASSERT_GT(objects.size(), 0) << "At least one object must be created.";
    
    // Проверяем, что можем получить имена объектов
    for (const auto& obj : objects) {
        EXPECT_FALSE(obj->getName().empty()) << "The object must have a name";
    }
    
    // Проверяем основные типы объектов
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
    
    EXPECT_TRUE(hasRectangle) << "There must be at least one rectangle.";
    EXPECT_TRUE(hasText) << "There must be at least one text.";
    EXPECT_TRUE(hasButton) << "There must be at least one button.";
    
    // Проверяем инициализацию переменных в базе данных
    EXPECT_TRUE(db.variableExists("temperature_value")) << "The variable temperature_value must exist";
    EXPECT_TRUE(db.variableExists("pressure_value")) << "There must be a presure_value variable";
    EXPECT_TRUE(db.variableExists("setpoint_value")) << "There must be a setpoint_value variable";
    EXPECT_TRUE(db.variableExists("panel_status")) << "There must be a panel_status variable.";
}

