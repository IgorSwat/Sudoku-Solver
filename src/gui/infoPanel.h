#pragma once

#include "resource.h"


namespace GUI {

    // ---------------
    // InfoPanel class
    // ---------------

    class InfoPanel : public sf::Drawable
    {
    public:
        InfoPanel(unsigned fontSize, float iconSpacing, float rankSpacing);

        void setCorrectness(bool correct) { correctness = correct; }
        void setSolveTime(int time) { solveTime = time; updateSolveLabel(); }

        void setPosition(sf::Vector2f pos) { position = pos; alignElements(); }
        sf::Vector2f getPosition() const { return position; }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        void updateSolveLabel();
        void alignElements();

        // Graphic content
        sf::Text correctnessLabel;
        sf::Text solveTimeLabel;
        sf::Sprite correctIcon;
        sf::Sprite incorrectIcon;

        // Logic
        sf::Vector2f position;
        bool correctness = true;
        int solveTime = -1;

        const float iconSpacing;
        const float rankSpacing;
    };

}