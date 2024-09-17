#pragma once

#include "button.h"
#include <vector>


namespace GUI {

	// ------------
	// Navbar class
	// ------------

	class Navbar : public sf::Drawable
	{
	public:
		Navbar(float buttonSpacing);

        void setPosition(sf::Vector2f pos) { position = pos; alignButtons(); }
        sf::Vector2f getPosition() const { return position; }

		// Manipulating button list
		// Allows to disable align of some button to place it outside the navbar
		void addButton(const LabeledButton& button, bool align = true, sf::Vector2f bPos = {0, 0});

		// Main state update, returns the clicked button (if any)
		ButtonType update(const sf::Event& event);

		// Draw
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		void alignButtons();

		// Graphic content
		std::vector<std::pair<LabeledButton, bool>> buttons;

		// Size and alignment
		sf::Vector2f position;
		const float buttonSpacing;
	};

}