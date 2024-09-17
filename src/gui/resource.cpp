#include "resource.h"
#include "button.h"
#include <iostream>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>


namespace GUI {

    // --------------
    // Resource paths
    // --------------

    const std::string RESOURCE_ROOT = std::string(PROJECT_ROOT) + "/resource/";

    const std::string TILE_FONT_PATH = RESOURCE_ROOT + "coolvetica rg.otf";
    const std::string BACK_ICON = RESOURCE_ROOT + "back-arrow.png";
    const std::string CLEAR_ICON = RESOURCE_ROOT + "reset.png";
    const std::string GENERATE_ICON = RESOURCE_ROOT + "sudoku-2.png";
    const std::string SOLVE_ICON = RESOURCE_ROOT + "check-symbol.png";
    const std::string CHECK_ICON = RESOURCE_ROOT + "check.png";
    const std::string X_ICON = RESOURCE_ROOT + "remove.png";

    constexpr Resource BUTTON_MAPPING[int(ButtonType::BUTTON_TYPE_RANGE)] = {
        Resource::NONE, Resource::BACK_BUTTON_ICON, Resource::CLEAR_BUTTON_ICON, Resource::GENERATE_BUTTON_ICON,
        Resource::SOLVE_BUTTON_ICON
    };


    // ---------
    // Resources
    // ---------

    bool loaded = false;

    std::unordered_map<Resource, std::unique_ptr<sf::Font>> fonts;
    std::unordered_map<Resource, std::unique_ptr<sf::Texture>> textures;


    // ----------------
    // Resource loading
    // ----------------

    // Load given texture and check for correctness
	void load_texture(Resource name, const std::string &path)
	{
		textures.insert({name, std::make_unique<sf::Texture>()});
		if (!textures[name]->loadFromFile(path)) {
			std::cerr << "Unable to load texture from: " << path << std::endl;
			exit(EXIT_FAILURE);
		}
	}

    void load_resources()
    {
        if (!loaded) {
            std::unique_ptr<sf::Font> tileFont = std::make_unique<sf::Font>();
            tileFont->loadFromFile(TILE_FONT_PATH);
            fonts.insert({Resource::MAIN_FONT, std::move(tileFont)});
            
            load_texture(Resource::BACK_BUTTON_ICON, BACK_ICON);
            load_texture(Resource::CLEAR_BUTTON_ICON, CLEAR_ICON);
            load_texture(Resource::GENERATE_BUTTON_ICON, GENERATE_ICON);
            load_texture(Resource::SOLVE_BUTTON_ICON, SOLVE_ICON);
            load_texture(Resource::GREEN_CHECK_ICON, CHECK_ICON);
            load_texture(Resource::RED_X_ICON, X_ICON);

            loaded = true;
        }
    }

    const sf::Font& get_font(Resource fontName)
    {
        if (!fonts.contains(fontName))
            throw std::invalid_argument("Font with identifier " + std::to_string(int(fontName)) + " not found");

        return *fonts[fontName];
    }

    const sf::Texture& get_texture(Resource textureName)
    {
        if (!textures.contains(textureName))
            throw std::invalid_argument("Texture with identifier " + std::to_string(int(textureName)) + " not found");

        return *textures[textureName];
    }

    const sf::Texture& get_texture(ButtonType bType)
    {
        return get_texture(BUTTON_MAPPING[int(bType)]);
    }

}