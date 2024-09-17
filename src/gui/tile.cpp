#include "tile.h"


namespace GUI {

    // ------------
    // Tile methods
    // ------------

    Tile::Tile()
        : background(sf::Vector2(0.f, 0.f))
    {
    }

    Tile::Tile(float tileSize, unsigned fontSize)
        : background(sf::Vector2f(tileSize, tileSize))
    {
        text.setFont(get_font(Resource::MAIN_FONT));
        text.setCharacterSize(fontSize);

        alignElements();
        setText();
        setTextColor(sf::Color::Black);
    }

    void Tile::alignElements()
    {
        background.setPosition(position);
        text.setPosition(sf::Vector2f(
            position.x + background.getGlobalBounds().width / 2.f - text.getLocalBounds().width / 2.f,
            position.y + background.getGlobalBounds().height / 2.f - text.getLocalBounds().height / 2.f - text.getLocalBounds().top
        ));
    }

    void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(background, states);
        target.draw(text, states);
    }

}