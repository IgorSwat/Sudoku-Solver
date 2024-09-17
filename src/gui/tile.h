#pragma once

#include "resource.h"


namespace GUI {

    // ----------
    // Tile class
    // ----------

    class Tile : public sf::Drawable
    {
    public:
        Tile();
        Tile(float tileSize, unsigned fontSize);
        Tile(const Tile& other) = default;
        Tile& operator=(const Tile& other) = default;

        void setNumber(int n) { num = n; setText(); }
        void removeNumber() { num = 0; setText(); }
        int getNumber() const { return num; }

        void setPosition(sf::Vector2f pos) { position = pos; alignElements(); }
        sf::Vector2f getPosition() const { return position; }
        sf::FloatRect getGlobalBounds() const { return background.getGlobalBounds(); }

        void setBackgroundColor(sf::Color color) { background.setFillColor(color); }
        void setTextColor(sf::Color color) { text.setFillColor(color); }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        void alignElements();
        void setText() { text.setString(num != 0 ? std::to_string(num) : ""); alignElements(); }

        // Graphic content
        sf::RectangleShape background;
        sf::Text text;

        // Tile parameters
        sf::Vector2f position = {0, 0};      // Top left corner
        int num = 0;                         // A number shown by 'text' element
    };

}