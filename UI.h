#ifndef __UI_H__
#define __UI_H__


struct UI {
	// Edge Width
	int marginTop;
	int marginLeft;

	// Number of game areas
	int gameWidth;
	int gameHeight;

	// Full window size width
	int windowWidth;
	int windowHeight;

	char *snakeBlock;	// Snake
	char *wallBlock;	// Wall
	char *foodBlock;	// Food
	int blockWidth;		// Each block should to be equal
};

struct UI * UIInitialize(int width, int height);                             // UI Initialize
void UIDisplayWizard(const struct UI *pUI);                                  // Display the game wizard
void UIDisplayGameWindow(const struct UI *pUI, int score, int scorePerFood); // Displays the game as a whole, including the wall, the information on the right
void UIDisplayFoodAtXY(const struct UI *pUI, int x, int y);                  // Show food at X,Y
void UIDisplaySnakeBlockAtXY(const struct UI *pUI, int x, int y);            // Display a node of a snake at X,Y
void UICleanBlockAtXY(const struct UI *pUI, int x, int y);                   // Clear out the display block at x, y
void UIDisplayScore(const struct UI *pUI, int score, int scorePerFood);      // Display score information
void UIShowMessage(const struct UI *pUI, const char *message);               // Display the game exit message in the middle
void UIDeinitialize(struct UI *pUI);                                         // Destroy UI resources


#endif