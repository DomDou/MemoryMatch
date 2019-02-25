#include <RGF/Game.h>
#include <RGFUtil/Utility.h>
#include "Card.h"
#include <string>
#include <iostream>

std::map<Card::Difficulty, std::vector<std::string>> Card::cardNames = 
{ {Card::Difficulty::EASY, { "food/bk", "food/dominos", "food/dq", "food/mcdo", "food/starbucks", "food/sub", "food/swiss", "food/tim" }},

{Card::Difficulty::MEDIUM, {"mario/birdo", "mario/boo", "mario/bowser", "mario/bowserjr", "mario/daisy", "mario/dk", "mario/goomba", "mario/koopa", 
"mario/luigi", "mario/mario", "mario/peach", "mario/rosalina", "mario/shyguy", "mario/thwomp", "mario/toad", "mario/waluigi", "mario/wario", "mario/yoshi"}},

{Card::Difficulty::HARD, {"nhl/avalanche", "nhl/blackhawks", "nhl/bluejackets", "nhl/blues", "nhl/bruins", "nhl/canadiens", "nhl/canucks", "nhl/capitals", 
"nhl/coyotes", "nhl/devils", "nhl/ducks", "nhl/flames",  "nhl/flyers", "nhl/goldenknights", "nhl/hurricanes", "nhl/islanders", "nhl/jets", "nhl/kings", 
"nhl/lightning", "nhl/mapleleafs", "nhl/nhl", "nhl/oilers", "nhl/panthers", "nhl/penguins", "nhl/predators", "nhl/rangers", "nhl/redwings", "nhl/sabres",
"nhl/senators", "nhl/sharks", "nhl/stars", "nhl/wild", }} };

std::map<Card::Difficulty, std::string> Card::cardBgs = 
{ { Card::Difficulty::EASY, "food/b" },
{ Card::Difficulty::MEDIUM, "mario/b" },
{ Card::Difficulty::HARD, "nhl/b" } };

Card::Card(rgf::Scene& owner, const std::string& card, const std::string& back)
	:rgf::Entity(owner),
	name(card)
{
	cardTexture = getOwner().getGame().getResourceManager().loadTexture(std::string("res/images/") + name + std::string(".png"));
	backTexture = getOwner().getGame().getResourceManager().loadTexture(std::string("res/images/") + back + std::string(".png"));
	cardSprite.setTexture(*backTexture);
}

Card::~Card()
{
}

std::vector<Card::Ptr> Card::generateDeck(rgf::Scene& owner, Difficulty diff)
{
	std::vector<Card::Ptr> vec;

	for (const auto& name : cardNames[diff])
	{
		vec.push_back(Card::Ptr(new Card(owner, name, cardBgs[diff])));
		vec.push_back(Card::Ptr(new Card(owner, name, cardBgs[diff])));
	}

	return vec;
}

sf::Vector2u Card::getSize() const
{
	return cardTexture->getSize();
}

std::string Card::getName() const
{
	return name;
}

Card::Difficulty Card::getDifficulty() const
{
	return difficulty;
}

void Card::setDifficulty(Card::Difficulty newDiff)
{
	difficulty = newDiff;
}

bool Card::isFaceDown() const
{
	return isDown;
}

bool Card::operator==(const Card& rhs) const
{
	return (name == rhs.name);
}

void Card::flipCard()
{
	cardSprite.setTexture(isDown ? *cardTexture : *backTexture);
	isDown = !isDown;
}

void Card::initCurrent()
{
	getOwner().getGame().getEventManager().subscribe(sf::Event::MouseButtonPressed, *this, [this](auto& data) {this->onMouseDown(data); });
}

void Card::updateCurrent(const sf::Time & dt)
{
}

void Card::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(cardSprite, states);
}

void Card::cleanupCurrent()
{
	getOwner().getGame().getEventManager().unsubscribe(sf::Event::MouseButtonPressed, *this);
}

void Card::onMouseDown(const rgf::EventData & data)
{
	if (isVisible())
	{
		auto click = data.getDataAs<sf::Event::MouseButtonEvent>();
		auto rect = getWorldTransform().transformRect(cardSprite.getGlobalBounds());
		auto mouseClick = sf::Vector2f(static_cast<float>(click.x), static_cast<float>(click.y));
		if (rect.contains(mouseClick) && click.button == sf::Mouse::Button::Left)
		{
			getOwner().getEventManager().postEvent(rgf::Event(CARD_PRESSED, this));
		}
	}
}