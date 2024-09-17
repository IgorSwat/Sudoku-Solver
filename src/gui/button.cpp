#include "button.h"
#include <cmath>


namespace GUI {

    // Customizable parameters
    constexpr std::size_t BACKGROUND_POINT_COUNT = 200;

    constexpr float BUTTON_OUTLINE_RATIO = 0.05f;
    constexpr float BUTTON_ICON_SIZE_RATIO = 0.7f;


    // -------------------------
	// Rounded rectangle methods
	// -------------------------

    RoundedRectangle::RoundedRectangle(float diameter, float round)
        : radius(diameter / 2.f), round(round)
    {
        update();
    }

    std::size_t RoundedRectangle::getPointCount() const
    {
        return BACKGROUND_POINT_COUNT;
    }

    sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const
    {
        static const float pi = 3.141592654f;

        const sf::Vector2f refPoint = getRefPoint(index);
        const float tranAngle = 4 * (pi / 2.f) / BACKGROUND_POINT_COUNT;

        float angle = static_cast<int>(4 * index / BACKGROUND_POINT_COUNT) * (pi / 2);
        index = index % (BACKGROUND_POINT_COUNT / 4);
        angle += index * tranAngle;

        // Transform basic rectangle point
        float x = std::cos(angle) * round * radius;
        float y = std::sin(angle) * round * radius;
        return sf::Vector2f(refPoint.x + x, refPoint.y - y);
    }

    sf::Vector2f RoundedRectangle::getRefPoint(std::size_t index) const
    {
        std::size_t part = 4 * index / BACKGROUND_POINT_COUNT;
        switch (part) {
            case 0:
                return sf::Vector2f((2.f - round) * radius, round * radius);
            case 1:
                return sf::Vector2f(round * radius, round * radius);
            case 2:
                return sf::Vector2f(round * radius, (2.f - round) * radius);
            case 3:
            default:
                return sf::Vector2f((2.f - round) * radius, (2.f - round) * radius);
        }
    }


    // -------------------------
	// InteractiveButton methods
	// -------------------------

    bool InteractiveButton::update(sf::Event::EventType eventType, bool onto)
    {
        // Left mouse click above the button, only the first one after reset
        if (eventType == sf::Event::MouseButtonPressed && !clicked) {
            onClick();
            clicked = true;

            // The only case when we should signal environment that action related to button click should happen
            return true;
        }
        // Mouse button released above the button
        else if (eventType == sf::Event::MouseButtonReleased) {
            onHover();
            clicked = false;
        }
        // Either moved onto the button, or moved away from the button (reset case)
        else if (eventType == sf::Event::MouseMoved) {
            if (onto && !checked)
                onHover();
            else if (!onto && (clicked || checked)) {
                clicked = false;
                onDefault();
            }
            checked = onto;
        }

        return false;
    }


    // ---------------------
	// RoundedButton methods
	// ---------------------

    RoundedButton::RoundedButton(ButtonType bType, float size, float round,
                                 sf::Color colorDef, sf::Color colorHover, sf::Color colorClick)
        : background(size, round), icon(get_texture(bType)), bType(bType),
          colorDef(colorDef), colorHover(colorHover), colorClick(colorClick)
    {
        background.setOutlineColor(sf::Color::Black);
        background.setOutlineThickness(size * BUTTON_OUTLINE_RATIO);

        scaleIcon();
        centralizeIcon();
    }

    void RoundedButton::centralizeIcon()
    {
        sf::Vector2f pos = background.getPosition();
        float radius = background.diameter() / 2.f;

        sf::Vector2f center = {
            pos.x + radius - icon.getGlobalBounds().width / 2.f,
            pos.y + radius - icon.getGlobalBounds().height / 2.f
        };
        icon.setPosition(center);
    }

    void RoundedButton::scaleIcon()
    {
        const sf::Texture& texture = get_texture(bType);
        const float textureSize = std::max(static_cast<float>(texture.getSize().x), static_cast<float>(texture.getSize().y));

        float factor = BUTTON_ICON_SIZE_RATIO * background.diameter() / textureSize;
        icon.setScale(factor, factor);
    }

    void RoundedButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(background, states);
     	target.draw(icon, states);
    }


    // ---------------------
	// LabeledButton methods
	// ---------------------

    LabeledButton::LabeledButton(ButtonType bType, std::string label, unsigned fontSize, float textSpacing, float bSize, float bRound,
                                 sf::Color colorDef, sf::Color colorHover, sf::Color colorClick)
        : RoundedButton(bType, bSize, bRound, colorDef, colorHover, colorClick), radius(bSize / 2), spacing(textSpacing)
    {
        text.setFont(get_font(Resource::MAIN_FONT));
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::Black);
        text.setString(label);

        alignElements();
    }

    void LabeledButton::alignElements()
    {
        text.setPosition(position);
        RoundedButton::setPosition(sf::Vector2f(
            position.x + text.getLocalBounds().width / 2 - radius,
            position.y + text.getLocalBounds().height + spacing
        ));
    }

    void LabeledButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(text, states);
        RoundedButton::draw(target, states);
    }

}