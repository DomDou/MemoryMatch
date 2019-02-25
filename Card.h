#ifndef _DECK_H_
#define _DECK_H_

#include <array>
#include <RGF/Entity.h>
#include <RGF/ResourceManager.h>
#include <RGF/EventData.h>
#include <vector>

class Card : public rgf::Entity
{
public:
	enum { CARD_PRESSED = 0x200A194A };

	using Ptr = std::shared_ptr <Card>;
	
	enum Difficulty {EASY, MEDIUM, HARD};

	~Card();

	static std::vector<Card::Ptr> generateDeck(rgf::Scene& owner, Difficulty diff);

	sf::Vector2u getSize() const;

	std::string getName() const;

	Card::Difficulty getDifficulty() const;
	void setDifficulty(Card::Difficulty newDiff);

	bool isFaceDown() const;
	void flipCard();

	bool operator==(const Card& rhs) const;

protected:
	void initCurrent() override;
	void updateCurrent(const sf::Time& dt) override;
	void drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const override;
	void cleanupCurrent() override;

private:
	static std::map<Difficulty, std::vector<std::string>> cardNames;
	static std::map<Difficulty,std::string> cardBgs;

	Card(rgf::Scene& owner, const std::string& card, const std::string& back);

	Card::Difficulty difficulty;

	void onMouseDown(const rgf::EventData& data);

	rgf::SharedTexture cardTexture;
	rgf::SharedTexture backTexture;

	sf::Sprite cardSprite;

	std::string name;

	bool isDown = true;
};

#endif