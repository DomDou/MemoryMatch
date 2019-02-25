#ifndef _MEMORY_SCENE_H_
#define _MEMORY_SCENE_H_

#include <vector>

#include <RGF/Scene.h>
#include <RGF/ResourceManager.h>
#include <RGF/EventData.h>
#include <RGFUtil/Timer.h>
#include <RGFUtil/Button.h>
#include <RGFUtil/Random.h>
#include <RGFUtil/SidePanel.h>
#include <RGF/DrawableNode.h>

#include "Card.h"

class MemoryScene : 
	public rgf::Scene
{
public:
	using Ptr = std::shared_ptr<MemoryScene>;

	MemoryScene(rgf::Game& game);
	virtual ~MemoryScene();

protected:
	void initCurrent() override;
	void updateCurrent(const sf::Time& dt) override;
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void cleanupCurrent() override;

private:
	rgf::SidePanel::Ptr panel;

	enum {
		BTN_EASY_PRESSED = 0xE3F7ED75,
		BTN_MEDIUM_PRESSED = 0xB8AAF7C9,
		BTN_HARD_PRESSED = 0xCF152283,
		DELAY_FLIP = 0x607DF753,
		DELAY_PAIR = 0x3CE45E48,
		WINNING_SCENARIO = 0xCDF68E37
	};

	void onEasyPressed();
	void onMediumPressed();
	void onHardPressed();
	void onCardPressed(const rgf::EventData& data);
	void onDelayFlip();
	void onDelayPair();

	void createDeck(Card::Difficulty diff);
	void resetGame(Card::Difficulty diff);
	void updateCurrentDifficulty(Card::Difficulty newDiff);
	void winScenario();

	void increaseTry();

	Card::Difficulty currentDiff;
	std::vector<Card::Ptr> deck;
	Card* firstCard;
	Card* secondCard;

	rgf::Button::Ptr btnEasy;
	rgf::Button::Ptr btnMedium;
	rgf::Button::Ptr btnHard;
	rgf::Random rng;

	rgf::SharedTexture texture;
	rgf::SharedTexture bgTexture;
	rgf::SharedFont btnFont;
	rgf::SharedFont lblFont;
	
	sf::Sprite bgSprite;
	sf::Text tryCounterText;
	sf::Text congratsText;
	sf::Text endScoreText;
	sf::Text highScoreTitle;
	sf::Text highScoreTime;
	sf::Text highScoreTries;

	rgf::Timer::Ptr timer;

	int tryCounter;
	int pairCount;

	bool isPair = false;

	const std::string xmlGroupTime = "time";
	const std::string xmlGroupTries = "tries";

	void printTriesWrapper(std::list<rgf::Score> outputList);
	void printTimeWrapper(std::list<rgf::Score> outputList);

	rgf::SharedTexture titleTexture;

	sf::Sprite winningSprite;
	rgf::DrawableSprite::Ptr winningDrawable;
};

#endif