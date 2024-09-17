#pragma once

#include "resource.h"


namespace GUI {

    // ------------
	// Button types
	// ------------

    enum class ButtonType {
        BACK = 1,
        CLEAR,
        GENERATE,
        SOLVE,

        NONE = 0,
        BUTTON_TYPE_RANGE = 5
    };


    // -----------------------
	// Rounded rectangle shape
	// -----------------------

    class RoundedRectangle : public sf::Shape
    {
    public:
        RoundedRectangle(float diameter, float round = 0.4f);

        // Getters
        float diameter() const { return radius * 2; }

        // Shape methods
        std::size_t getPointCount() const override;
	    sf::Vector2f getPoint(std::size_t index) const override;

    private:
        sf::Vector2f getRefPoint(std::size_t index) const;  // Obtains a full rectangle point (without curves)

        const float radius;
        const float round;
    };


    // ----------------------------
	// Interactive button interface
	// ----------------------------

    // It changes appearence between default, appearence on hover and appearence on click
    class InteractiveButton
    {
    public:
        InteractiveButton() = default;
        virtual ~InteractiveButton() = default;

        // Main method to interact with user events
        bool update(sf::Event::EventType eventType, bool onto = true);      // Returns true if button is clicked (for the first time after reset)

        // Behavior on different types of events
        virtual void onDefault() = 0;
        virtual void onHover() = 0;
        virtual void onClick() = 0;
    
    protected:
        // Button state
        bool checked = false;
        bool clicked = false;
    };


    // ------------
	// Button class
	// ------------

    class RoundedButton : public sf::Drawable, public InteractiveButton
    {
    public:
        RoundedButton(ButtonType bType, float size, float round,
                      sf::Color colorDef, sf::Color colorHover, sf::Color colorClick);

        // Position change
        virtual void setPosition(sf::Vector2f pos) { background.setPosition(pos); centralizeIcon(); }

        // Behavior on different types of events
        void onDefault() override { background.setFillColor(colorDef); }
        void onHover() override { background.setFillColor(colorHover); }
        void onClick() override { background.setFillColor(colorClick); }

        // Draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        // Getters
        ButtonType type() const { return bType; }
        sf::FloatRect getGlobalBounds() const {return background.getGlobalBounds();}

    private:
        void centralizeIcon();
        void scaleIcon();
        
        // Graphic content
        RoundedRectangle background;
        sf::Sprite icon;

        // Logic
        ButtonType bType;

        const sf::Color colorDef;
        const sf::Color colorHover;
        const sf::Color colorClick;
    };


    // -----------------
	// NotedButton class
	// -----------------

    class LabeledButton : public RoundedButton
    {
    public:
        LabeledButton(ButtonType bType, std::string label, unsigned fontSize, float textSpacing, float bSize, float bRound,
                      sf::Color colorDef, sf::Color colorHover, sf::Color colorClick);
        
        void setPosition(sf::Vector2f pos) override { position = pos; alignElements(); }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    private:
        void alignElements();

        // Graphic content
        sf::Text text;

        // Logic
        sf::Vector2f position = {0, 0};

        const float radius;
        const float spacing;
    };

}