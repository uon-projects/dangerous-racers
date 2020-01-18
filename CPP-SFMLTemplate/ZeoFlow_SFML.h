/*
 * File:   MD2.h
 * Author: Teodor Grigor (original version)
 *
 * Class for loading the assets and format the background
 * For SFML2.

 (c) 2019 - ZeoFlow, Ltd
 
 This software is provided 'as-is', without any express or
 implied warranty. In no event will the authors be held
 liable for any damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute
 it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented;
 you must not claim that you wrote the original software.
 If you use this software in a product, an acknowledgment
 in the product documentation would be appreciated but
 is not required.
 
 2. Altered source versions must be plainly marked as such,
 and must not be misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any
 source distribution.
 
 *
 * Created on 30 January 2009, 11:02
 */

#include <SFML/Graphics.hpp>
#include <string>
#include <list>


namespace zeoFlow
{
	class ZeoFlow_SFML
	{
		std::list<sf::Texture> zfTextures;

	public:
		sf::Sprite loadSpriteFromTexture(std::string, std::string, std::string);

	public:
		sf::Sprite formatSpriteForBackground(sf::Sprite);

	public:
		sf::Sprite spriteInMiddle(sf::Sprite, double, double);

	public:
		sf::Font loadFont(std::string, std::string, std::string);

	public:
		sf::Text drawBtnText(std::string, int, int, int, sf::Font);

	public:
		void drawBtn(sf::Text);

	};

	sf::Sprite ZeoFlow_SFML::loadSpriteFromTexture(std::string assetPath, std::string assetName, std::string assetExtension)
	{
		sf::Texture zfTexture;
		if (!zfTexture.loadFromFile(assetPath + assetName + "." + assetExtension)) {
				system("pause");
		}
		zfTextures.push_front(zfTexture);
		sf::Sprite zfSprite(zfTextures.front());
		return zfSprite;
	}

	sf::Sprite ZeoFlow_SFML::formatSpriteForBackground(sf::Sprite assetSprite)
	{
		float BackgroundCurrentScaleX = 1.0f,BackgroundCurrentScaleY = 1.0f;
		float BackgroundWidth,BackgroundHeight;
		sf::Vector2f BackgroundLocation(0,0);
		assetSprite.setScale(sf::Vector2f(BackgroundCurrentScaleX, BackgroundCurrentScaleY));
		BackgroundWidth = assetSprite.getGlobalBounds().width;
		BackgroundHeight = assetSprite.getGlobalBounds().height;
		assetSprite.setPosition(BackgroundLocation.x, BackgroundLocation.y);
		return assetSprite;
	}

	sf::Sprite ZeoFlow_SFML::spriteInMiddle(sf::Sprite assetSprite, double windowWidth, double windowHeight)
	{
		double windowHeightHalf = windowHeight / 2;
		double windowWidthHalf = windowWidth / 2;
		double spriteHeightHalf = assetSprite.getGlobalBounds().width / 2;
		double spriteWidthHalf = assetSprite.getGlobalBounds().width / 2;
		assetSprite.setPosition(windowWidthHalf - spriteWidthHalf, windowHeightHalf - spriteHeightHalf);
		return assetSprite;
	}

	sf::Font ZeoFlow_SFML::loadFont(std::string assetPath, std::string assetName, std::string assetExtension)
	{
		sf::Font font;
		if (!font.loadFromFile(assetPath + assetName + "." + assetExtension)) {
				system("pause");
		}
		return font;
	}

	sf::Text ZeoFlow_SFML::drawBtnText(std::string title, int characterSize, int x, int y, sf::Font font)
	{
		sf::Text btnText;

		btnText.setString(title);
		btnText.setFont(font);
		btnText.setPosition(x, y);
		btnText.setCharacterSize(characterSize);
		btnText.setFillColor(sf::Color::White);
		btnText.setOrigin(btnText.getGlobalBounds().width/2, btnText.getGlobalBounds().height/2);

		return btnText;

	}

	void ZeoFlow_SFML::drawBtn(sf::Text btnText)
	{
		sf::RectangleShape btnPlayL1, btnPlayL2, btnPlayL3;

		btnPlayL1.setSize(sf::Vector2f(btnText.getLocalBounds().width + 30, btnText.getLocalBounds().height  + 20));
		btnPlayL2.setSize(sf::Vector2f(btnText.getLocalBounds().width + 30, btnText.getLocalBounds().height  + 20));
		btnPlayL3.setSize(sf::Vector2f(btnText.getLocalBounds().width + 30, btnText.getLocalBounds().height  + 20));

		btnPlayL1.setOrigin(btnPlayL1.getGlobalBounds().width/2, btnPlayL1.getGlobalBounds().height/2);
		btnPlayL2.setOrigin(btnPlayL2.getGlobalBounds().width/2, btnPlayL2.getGlobalBounds().height/2);
		btnPlayL3.setOrigin(btnPlayL3.getGlobalBounds().width/2, btnPlayL3.getGlobalBounds().height/2);
	
		btnPlayL1.setFillColor(sf::Color(26,35,126));
		btnPlayL2.setFillColor(sf::Color(136,14,79));
		btnPlayL3.setFillColor(sf::Color(74,20,140));

	}
}