#include "infoPanel.h"


namespace GUI {

    const float FONT_SCALE_FACTOR = 1.0f;


    // -----------------
    // InfoPanel methods
    // -----------------

    InfoPanel::InfoPanel(unsigned fontSize, float iconSpacing, float rankSpacing)
        : iconSpacing(iconSpacing), rankSpacing(rankSpacing)
    {
        correctnessLabel.setFont(get_font(Resource::MAIN_FONT));
        correctnessLabel.setCharacterSize(fontSize);
        correctnessLabel.setFillColor(sf::Color::Black);
        correctnessLabel.setString("Correctness:");

        solveTimeLabel.setFont(get_font(Resource::MAIN_FONT));
        solveTimeLabel.setCharacterSize(fontSize);
        solveTimeLabel.setFillColor(sf::Color::Black);
        updateSolveLabel();

        const sf::Texture& checkIconTexture = get_texture(Resource::GREEN_CHECK_ICON);
        float scaleFactor = FONT_SCALE_FACTOR * fontSize / checkIconTexture.getSize().x;
        correctIcon.setTexture(checkIconTexture);
        correctIcon.scale(scaleFactor, scaleFactor);
        correctIcon.setOrigin(correctIcon.getLocalBounds().width / 2, correctIcon.getLocalBounds().height / 2);

        const sf::Texture& xIconTexture = get_texture(Resource::RED_X_ICON);
        scaleFactor = FONT_SCALE_FACTOR * fontSize / xIconTexture.getSize().x;
        incorrectIcon.setTexture(xIconTexture);
        incorrectIcon.scale(scaleFactor, scaleFactor);
        incorrectIcon.setOrigin(incorrectIcon.getLocalBounds().width / 2, incorrectIcon.getLocalBounds().height / 2);

        alignElements();
    }

    void InfoPanel::updateSolveLabel()
    {
        std::string label = "Solve time: ";
        if (solveTime >= 0)
            label += std::to_string(solveTime) + " ms";
        
        solveTimeLabel.setString(label);
    }

    void InfoPanel::alignElements()
    {
        correctnessLabel.setPosition(position);
        correctIcon.setPosition(position.x + correctnessLabel.getLocalBounds().width + iconSpacing + correctIcon.getGlobalBounds().width / 2, 
                                position.y + correctnessLabel.getLocalBounds().height / 2 + correctnessLabel.getLocalBounds().top);
        incorrectIcon.setPosition(position.x + correctnessLabel.getLocalBounds().width + iconSpacing + incorrectIcon.getGlobalBounds().width / 2, 
                                  position.y + correctnessLabel.getLocalBounds().height / 2 + correctnessLabel.getLocalBounds().top);

        solveTimeLabel.setPosition(position.x, position.y + correctnessLabel.getLocalBounds().height + rankSpacing);
    }

    void InfoPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(correctnessLabel, states);
        target.draw(solveTimeLabel, states);
        target.draw(correctness ? correctIcon : incorrectIcon, states);
    }

}