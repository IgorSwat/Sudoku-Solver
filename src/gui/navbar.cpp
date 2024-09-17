#include "navbar.h"


namespace GUI {

	// --------------
	// Navbar methods
	// --------------

	Navbar::Navbar(float buttonSpacing)
		: buttonSpacing(buttonSpacing)
	{
	}

	void Navbar::addButton(const LabeledButton& button, bool align, sf::Vector2f bPos)
	{
		buttons.push_back({button, align});

		if (!align)
			buttons.back().first.setPosition(bPos);
		alignButtons();
	}

	ButtonType Navbar::update(const sf::Event& event)
	{
		ButtonType result = ButtonType::NONE;

		// Navbar accepts only mouse left clicks
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button != sf::Mouse::Left)
			return result;

        sf::Vector2f mousePos = event.type == sf::Event::MouseMoved ? sf::Vector2f(float(event.mouseMove.x), float(event.mouseMove.y)) :
                                                                      sf::Vector2f(float(event.mouseButton.x), float(event.mouseButton.y));

		for (auto& buttonInfo : buttons) {
			LabeledButton& button = buttonInfo.first;
			if (buttonInfo.first.getGlobalBounds().contains(mousePos)) {
				if (button.update(event.type, true))
					result = button.type();
			}
			else
				button.update(sf::Event::MouseMoved, false);
		}

		return result;
	}

	void Navbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (const auto& buttonInfo : buttons)
			target.draw(buttonInfo.first, states);
	}

	void Navbar::alignButtons()
	{
        sf::Vector2f pos = position;
		
		for (auto& buttonInfo: buttons) {
			if (!buttonInfo.second)
				continue;

			LabeledButton& button = buttonInfo.first;
			button.setPosition(pos);

            sf::Vector2f shift = {button.getGlobalBounds().width + buttonSpacing, 0.f};
            pos += shift;
		}
	}

}