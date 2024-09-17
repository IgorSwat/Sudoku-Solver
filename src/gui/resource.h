#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


namespace GUI {

    enum class ButtonType;


    // ----------------
    // Resource marking
    // ----------------

    enum class Resource {
        MAIN_FONT,

        BACK_BUTTON_ICON,
        CLEAR_BUTTON_ICON,
        GENERATE_BUTTON_ICON,
        SOLVE_BUTTON_ICON,

        GREEN_CHECK_ICON,
        RED_X_ICON,

        NONE = 0
    };


    // ----------------
    // Resource loading
    // ----------------

    extern const std::string RESOURCE_ROOT;

    void load_resources();

    const sf::Font& get_font(Resource fontName);
    const sf::Texture& get_texture(Resource textureName);
    const sf::Texture& get_texture(ButtonType bType);
    
}