#include "colors.h"
#include "CustomException.h"
#include "SettingsScreen.h"
#include "MainGame.h"
#include <iostream>

typedef irr::core::rect< irr::s32 > Rectangle;

SettingsScreen::SettingsScreen() {
	restartNotice = L"Some of these settings will only take effect when the game is closed & reopened.";
	setPointers( nullptr, nullptr, nullptr, nullptr, nullptr );
	backgroundColor = LIGHTGRAY;
	settingsChanged = false;
}

SettingsScreen::~SettingsScreen() {
	//dtor
}

void SettingsScreen::backToMenu() {
	mainGame->currentScreen = MainGame::MENUSCREEN;
	changeFromSettingsScreen();
}

//TODO: Add tooltips based on Settings Manager's defaults and comments.
void SettingsScreen::changeToSettingsScreen() {
	try {
		if( device == nullptr or device == NULL ) {
			CustomException e( L"device is null" );
			throw e;
		}
		
		settingsChanged = false;
		
		driver = device->getVideoDriver();
		environment = device->getGUIEnvironment();
		skin = environment->getSkin();
		
		if( textFont != nullptr ) {
			skin->setFont( textFont );
		}
		
		auto font = skin->getFont();
		
		{
			auto textDimensions = font->getDimension( restartNotice.c_str() );
			if( textDimensions.Width > mainGame->getScreenSize().Width ) {
				textDimensions.Width = mainGame->getScreenSize().Width;
				textDimensions.Height = textDimensions.Height * 2;
			}
			auto textRectangle = Rectangle( 0, 0, textDimensions.Width, textDimensions.Height );
			environment->addStaticText( restartNotice.c_str(), textRectangle );
			
			auto buttonsY = textDimensions.Height + 1;
			decltype( buttonsY ) tabsY;
			
			{ //Buttons above tab bar
				auto buttonWidth = mainGame->getScreenSize().Width / 4;
				auto buttonHeight = 30; //Not sure how tall these buttons should be; this is just a guess.
				
				auto cancelButtonRectangle = Rectangle( 0, buttonsY, 0 + buttonWidth, buttonsY + buttonHeight );
				cancelButton = environment->addButton( cancelButtonRectangle, 0, CANCEL_ID, L"Cancel" );
				
				auto okButtonRectangle = Rectangle( cancelButtonRectangle.LowerRightCorner.X , cancelButtonRectangle.UpperLeftCorner.Y, cancelButtonRectangle.LowerRightCorner.X + buttonWidth, cancelButtonRectangle.UpperLeftCorner.Y + buttonHeight );
				okButton = environment->addButton( okButtonRectangle, 0, OK_ID, L"OK" );
				
				auto resetToDefaultsButtonRectangle = Rectangle( okButtonRectangle.LowerRightCorner.X , okButtonRectangle.UpperLeftCorner.Y, okButtonRectangle.LowerRightCorner.X + buttonWidth, okButtonRectangle.UpperLeftCorner.Y + buttonHeight );
				resetToDefaultsButton = environment->addButton( resetToDefaultsButtonRectangle, 0, RESET_TO_DEFAULTS_ID, L"Reset to defaults" );
				
				auto undoChangesButtonRectangle = Rectangle( resetToDefaultsButtonRectangle.LowerRightCorner.X , resetToDefaultsButtonRectangle.UpperLeftCorner.Y, resetToDefaultsButtonRectangle.LowerRightCorner.X + buttonWidth, resetToDefaultsButtonRectangle.UpperLeftCorner.Y + buttonHeight );
				undoChangesButton = environment->addButton( undoChangesButtonRectangle, 0, UNDO_CHANGES_ID, L"Undo changes" );
				
				tabsY = buttonsY + buttonHeight + 1;
			}
			
			{ //Tab bar
				Rectangle tabControlRectangle( 0, tabsY, mainGame->getScreenSize().Width, mainGame->getScreenSize().Height );
				tabControl = environment->addTabControl( tabControlRectangle, 0, false, true, TAB_CONTROL_ID );
				
				soundTab = tabControl->addTab( L"Sound" );
				graphicsTab = tabControl->addTab( L"Graphics" );
				multiplayerTab = tabControl->addTab( L"Multiplayer" );
				miscTab = tabControl->addTab( L"Miscellaneous" );
			}
			
			{ //Sound tab
				decltype( mainGame->getScreenSize().Height ) itemY = 0;
				
				irr::core::stringw buttonText = L"Play music";
				auto buttonTextDimensions = font->getDimension( buttonText.c_str() );
				auto MusicBoxRectangle = Rectangle( 0, itemY, buttonTextDimensions.Width + 30, itemY + buttonTextDimensions.Height ); //I measured the width of a checkbox as 18 pixels, plus an additional 6 pixels of space between that and the text. Then I upped it to 30 just to leave some room. Feel free to up it some more.
				playMusicCheckBox = environment->addCheckBox( settingsManager->getPlayMusic(), MusicBoxRectangle, soundTab, PLAY_MUSIC_CHECKBOX_ID, buttonText.c_str() );
				
				irr::core::stringw volumeTextString = L"Volume";
				auto volumeTextDimensions = font->getDimension( volumeTextString.c_str() );
				auto volumeTextRectangle = Rectangle( MusicBoxRectangle.LowerRightCorner.X + 1, itemY, MusicBoxRectangle.LowerRightCorner.X + 1 + volumeTextDimensions.Width, itemY + volumeTextDimensions.Height );
				volumeText = environment->addStaticText( volumeTextString.c_str(), volumeTextRectangle, false, true, soundTab );
				volumeText->setEnabled( playMusicCheckBox->isChecked() );
				
				auto volumeBarRectangle = Rectangle( volumeTextRectangle.LowerRightCorner.X + 1, itemY, mainGame->getScreenSize().Width, volumeTextRectangle.LowerRightCorner.Y );
				volumeBar = environment->addScrollBar( true, volumeBarRectangle, soundTab, VOLUME_BAR_ID );
				volumeBar->setEnabled( playMusicCheckBox->isChecked() );
				volumeBar->setMax( 100 );
				volumeBar->setMin( 0 );
				volumeBar->setPos( settingsManager->getMusicVolume() );
				
				itemY += buttonTextDimensions.Height + 1;
			}
			
			{ //Graphics tab
				decltype( mainGame->getScreenSize().Height ) itemY = 0;
				
				irr::core::stringw checkboxText = L"Fullscreen";
				auto cbTextDimensions = font->getDimension( checkboxText.c_str() );
				auto fullscreenBoxRectangle = Rectangle( 0, itemY, cbTextDimensions.Width + 30, itemY + cbTextDimensions.Height );
				fullscreenCheckBox = environment->addCheckBox( settingsManager->fullscreen, fullscreenBoxRectangle, graphicsTab, FULLSCREEN_CHECKBOX_ID, checkboxText.c_str() );
				
				irr::core::stringw autoDetectResolutionText = L"Autodetect resolution";
				auto autoDetectResolutionTextDimensions = font->getDimension( autoDetectResolutionText.c_str() );
				auto autoDetectResolutionRectangle = Rectangle( fullscreenBoxRectangle.LowerRightCorner.X, fullscreenBoxRectangle.UpperLeftCorner.Y, fullscreenBoxRectangle.LowerRightCorner.X + autoDetectResolutionTextDimensions.Width + 30, fullscreenBoxRectangle.UpperLeftCorner.Y + autoDetectResolutionTextDimensions.Height );
				autoDetectResolutionCheckBox = environment->addCheckBox( settingsManager->autoDetectFullscreenResolution, autoDetectResolutionRectangle, graphicsTab, AUTODETECT_RESOLUTION_CHECKBOX_ID, autoDetectResolutionText.c_str() );
				autoDetectResolutionCheckBox->setEnabled( fullscreenCheckBox->isChecked() );
				
				irr::core::stringw fullscreenResolutionTextString = L"Resolution";
				auto fullscreenResolutionTextDimensions = font->getDimension( fullscreenResolutionTextString.c_str() );
				auto fullscreenResolutionTextRectangle = Rectangle( autoDetectResolutionRectangle.LowerRightCorner.X, autoDetectResolutionRectangle.UpperLeftCorner.Y, autoDetectResolutionRectangle.LowerRightCorner.X + fullscreenResolutionTextDimensions.Width, autoDetectResolutionRectangle.UpperLeftCorner.Y + fullscreenResolutionTextDimensions.Height );
				fullscreenResolutionText = environment->addStaticText( fullscreenResolutionTextString.c_str(), fullscreenResolutionTextRectangle, false, true, graphicsTab );
				fullscreenResolutionText->setEnabled( fullscreenCheckBox->isChecked() );
				
				irr::core::stringw fullscreenWidthTextString = L"9999"; //Four digits should provide enough room
				auto fullscreenWidthDimensions = font->getDimension( fullscreenWidthTextString.c_str() );
				fullscreenWidthDimensions.Width *= 2; //No idea why, but Irrlicht 1.8 seems to ignore the width if we don't do this
				auto fullscreenWidthRectangle = Rectangle( fullscreenResolutionTextRectangle.LowerRightCorner.X, fullscreenResolutionTextRectangle.UpperLeftCorner.Y, fullscreenResolutionTextRectangle.LowerRightCorner.X + fullscreenWidthDimensions.Width, fullscreenResolutionTextRectangle.UpperLeftCorner.Y + fullscreenWidthDimensions.Height );
				{
					StringConverter sc;
					fullscreenWidthSpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->getWindowSize().Width ).c_str(), fullscreenWidthRectangle, true, graphicsTab, FULLSCREEN_WIDTH_SPINBOX_ID );
				}
				fullscreenWidthSpinBox->setEnabled( autoDetectResolutionCheckBox->isEnabled() && !autoDetectResolutionCheckBox->isChecked() );
				fullscreenWidthSpinBox->setDecimalPlaces( 0 );
				fullscreenWidthSpinBox->setStepSize( 1.0 );
				if( settingsManager->allowSmallSize ) {
					fullscreenWidthSpinBox->setRange( 0, 4294967295 ); //4,294,967,295 is the max a 32-bit unsigned integer can be
				} else {
					fullscreenWidthSpinBox->setRange( settingsManager->getMinimumWindowSize().Width, 4294967295 );
				}
				
				irr::core::stringw fullscreenHeightTextString = L"9999"; //Four digits should provide enough room
				auto fullscreenHeightDimensions = font->getDimension( fullscreenHeightTextString.c_str() );
				fullscreenHeightDimensions.Width *= 2; //No idea why, but Irrlicht 1.8 seems to ignore the width if we don't do this
				auto fullscreenHeightRectangle = Rectangle( fullscreenWidthRectangle.LowerRightCorner.X, fullscreenWidthRectangle.UpperLeftCorner.Y, fullscreenWidthRectangle.LowerRightCorner.X + fullscreenHeightDimensions.Width, fullscreenWidthRectangle.UpperLeftCorner.Y + fullscreenHeightDimensions.Height );
				{
					StringConverter sc;
					fullscreenHeightSpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->getWindowSize().Height ).c_str(), fullscreenHeightRectangle, true, graphicsTab, FULLSCREEN_HEIGHT_SPINBOX_ID );
				}
				fullscreenHeightSpinBox->setEnabled( autoDetectResolutionCheckBox->isEnabled() && !autoDetectResolutionCheckBox->isChecked() );
				fullscreenHeightSpinBox->setDecimalPlaces( 0 );
				fullscreenHeightSpinBox->setStepSize( 1.0 );
				if( settingsManager->allowSmallSize ) {
					fullscreenHeightSpinBox->setRange( 0, 4294967295 ); //4,294,967,295 is the max a 32-bit unsigned integer can be
				} else {
					fullscreenHeightSpinBox->setRange( settingsManager->getMinimumWindowSize().Height, 4294967295 );
				}
				
				itemY = fullscreenHeightRectangle.LowerRightCorner.Y + 1;
				
				irr::core::stringw bppTextString = L"Bits per pixel";
				auto bppTextDimensions = font->getDimension( bppTextString.c_str() );
				auto bppTextRectangle = Rectangle( 0, itemY, bppTextDimensions.Width, itemY + bppTextDimensions.Height );
				bppText = environment->addStaticText( bppTextString.c_str(), bppTextRectangle, false, true, graphicsTab );
				bppText->setEnabled( fullscreenCheckBox->isChecked() );
				
				irr::core::stringw bpp16Text = L"16";
				auto bpp16TextDimensions = font->getDimension( bpp16Text.c_str() );
				auto bpp16TextRectangle = Rectangle( bppTextRectangle.LowerRightCorner.X, bppTextRectangle.UpperLeftCorner.Y, bppTextRectangle.LowerRightCorner.X + bpp16TextDimensions.Width + 30, bppTextRectangle.UpperLeftCorner.Y + bpp16TextDimensions.Height );
				bpp16CheckBox = environment->addCheckBox( ( settingsManager->getBitsPerPixel() == 16 ), bpp16TextRectangle, graphicsTab, BPP16_CHECKBOX_ID, bpp16Text.c_str() );
				bpp16CheckBox->setEnabled( fullscreenCheckBox->isChecked() );
				
				irr::core::stringw bpp32Text = L"32";
				auto bpp32TextDimensions = font->getDimension( bpp32Text.c_str() );
				auto bpp32TextRectangle = Rectangle( bpp16TextRectangle.LowerRightCorner.X, bpp16TextRectangle.UpperLeftCorner.Y, bpp16TextRectangle.LowerRightCorner.X + bpp32TextDimensions.Width + 30, bpp16TextRectangle.UpperLeftCorner.Y + bpp32TextDimensions.Height );
				bpp32CheckBox = environment->addCheckBox( ( !bpp16CheckBox->isChecked() ), bpp32TextRectangle, graphicsTab, BPP32_CHECKBOX_ID, bpp32Text.c_str() );
				bpp32CheckBox->setEnabled( fullscreenCheckBox->isChecked() );
				
				itemY = bpp32TextRectangle.LowerRightCorner.Y + 1;
				
				//TODO: Add a setting for full-screen video mode? If so, best to eliminate the above bpp checkboxes, use device->getVideoModeList() to populate a IGUIListBox.
				
				irr::core::stringw windowSizeTextString = L"Window size";
				auto windowSizeTextDimensions = font->getDimension( windowSizeTextString.c_str() );
				auto windowSizeTextRectangle = Rectangle( 0, itemY, 0 + windowSizeTextDimensions.Width, itemY + windowSizeTextDimensions.Height );
				windowSizeText = environment->addStaticText( windowSizeTextString.c_str(), windowSizeTextRectangle, false, true, graphicsTab );
				windowSizeText->setEnabled( !fullscreenCheckBox->isChecked() );
				
				irr::core::stringw windowWidthTextString = L"9999"; //Four digits should provide enough room
				auto windowWidthDimensions = font->getDimension( windowWidthTextString.c_str() );
				windowWidthDimensions.Width *= 2; //No idea why, but Irrlicht 1.8 seems to ignore the width if we don't do this
				auto windowWidthRectangle = Rectangle( windowSizeTextRectangle.LowerRightCorner.X, windowSizeTextRectangle.UpperLeftCorner.Y, windowSizeTextRectangle.LowerRightCorner.X + windowWidthDimensions.Width, windowSizeTextRectangle.UpperLeftCorner.Y + windowWidthDimensions.Height );
				{
					StringConverter sc;
					windowWidthSpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->getWindowSize().Width ).c_str(), windowWidthRectangle, true, graphicsTab, WINDOW_WIDTH_SPINBOX_ID );
				}
				windowWidthSpinBox->setEnabled( !fullscreenCheckBox->isChecked() );
				windowWidthSpinBox->setDecimalPlaces( 0 );
				windowWidthSpinBox->setStepSize( 1.0 );
				if( settingsManager->allowSmallSize ) {
					windowWidthSpinBox->setRange( 0, 4294967295 ); //4,294,967,295 is the max a 32-bit unsigned integer can be
				} else {
					windowWidthSpinBox->setRange( settingsManager->getMinimumWindowSize().Width, 4294967295 );
				}
				
				irr::core::stringw windowHeightTextString = L"9999"; //Four digits should provide enough room
				auto windowHeightDimensions = font->getDimension( windowHeightTextString.c_str() );
				windowHeightDimensions.Width *= 2; //No idea why, but Irrlicht 1.8 seems to ignore the width if we don't do this
				auto windowHeightRectangle = Rectangle( windowWidthRectangle.LowerRightCorner.X, windowWidthRectangle.UpperLeftCorner.Y, windowWidthRectangle.LowerRightCorner.X + windowHeightDimensions.Width, windowWidthRectangle.UpperLeftCorner.Y + windowHeightDimensions.Height );
				{
					StringConverter sc;
					windowHeightSpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->getWindowSize().Height ).c_str(), windowHeightRectangle, true, graphicsTab, WINDOW_HEIGHT_SPINBOX_ID );
				}
				windowHeightSpinBox->setEnabled( !fullscreenCheckBox->isChecked() );
				windowHeightSpinBox->setDecimalPlaces( 0 );
				windowHeightSpinBox->setStepSize( 1.0 );
				if( settingsManager->allowSmallSize ) {
					windowHeightSpinBox->setRange( 0, 4294967295 ); //4,294,967,295 is the max a 32-bit unsigned integer can be
				} else {
					windowHeightSpinBox->setRange( settingsManager->getMinimumWindowSize().Height, 4294967295 );
				}
				
				itemY = windowHeightRectangle.LowerRightCorner.Y + 1;
				
				irr::core::stringw vsyncText = L"Vertical sync";
				auto vsyncTextDimensions = font->getDimension( vsyncText.c_str() );
				auto vsyncTextRectangle = Rectangle( 0, itemY, 0 + vsyncTextDimensions.Width + 30, itemY + vsyncTextDimensions.Height );
				vsyncCheckBox = environment->addCheckBox( settingsManager->vsync, vsyncTextRectangle, graphicsTab, VSYNC_CHECKBOX_ID, vsyncText.c_str() );
				
				itemY = vsyncTextRectangle.LowerRightCorner.Y + 1;
				irr::core::stringw driverTypeTextString = L"Driver type";
				auto driverTypeTextDimensions = font->getDimension( driverTypeTextString.c_str() );
				auto driverTypeTextRectangle = Rectangle( 0, itemY, 0 + driverTypeTextDimensions.Width, itemY + driverTypeTextDimensions.Height );
				driverTypeText = environment->addStaticText( driverTypeTextString.c_str(), driverTypeTextRectangle, false, true, graphicsTab );
				
				irr::core::stringw openGLTextString = L"OpenGL";
				auto openGLTextDimensions = font->getDimension( openGLTextString.c_str() );
				auto openGLTextRectangle = Rectangle( driverTypeTextRectangle.LowerRightCorner.X, driverTypeTextRectangle.UpperLeftCorner.Y, driverTypeTextRectangle.LowerRightCorner.X + openGLTextDimensions.Width + 30, driverTypeTextRectangle.UpperLeftCorner.Y + openGLTextDimensions.Height );
				openGLCheckBox = environment->addCheckBox( ( settingsManager->driverType == irr::video::EDT_OPENGL ), openGLTextRectangle, graphicsTab, OPENGL_CHECKBOX_ID, openGLTextString.c_str() );
				openGLCheckBox->setEnabled( device->isDriverSupported( irr::video::EDT_OPENGL ) );
				
				irr::core::stringw d3d9TextString = L"Direct3D 9";
				auto d3d9TextDimensions = font->getDimension( d3d9TextString.c_str() );
				auto d3d9TextRectangle = Rectangle( openGLTextRectangle.LowerRightCorner.X, openGLTextRectangle.UpperLeftCorner.Y, openGLTextRectangle.LowerRightCorner.X + d3d9TextDimensions.Width + 30, openGLTextRectangle.UpperLeftCorner.Y + d3d9TextDimensions.Height );
				direct3D9CheckBox = environment->addCheckBox( ( settingsManager->driverType == irr::video::EDT_DIRECT3D9 ), d3d9TextRectangle, graphicsTab, DIRECT3D9_CHECKBOX_ID, d3d9TextString.c_str() );
				direct3D9CheckBox->setEnabled( device->isDriverSupported( irr::video::EDT_DIRECT3D9 ) );
				
				irr::core::stringw d3d8TextString = L"Direct3D 8";
				auto d3d8TextDimensions = font->getDimension( d3d8TextString.c_str() );
				auto d3d8TextRectangle = Rectangle( d3d9TextRectangle.LowerRightCorner.X, d3d9TextRectangle.UpperLeftCorner.Y, d3d9TextRectangle.LowerRightCorner.X + d3d8TextDimensions.Width + 30, d3d9TextRectangle.UpperLeftCorner.Y + d3d8TextDimensions.Height );
				direct3D8CheckBox = environment->addCheckBox( ( settingsManager->driverType == irr::video::EDT_DIRECT3D8 ), d3d8TextRectangle, graphicsTab, DIRECT3D8_CHECKBOX_ID, d3d8TextString.c_str() );
				direct3D8CheckBox->setEnabled( device->isDriverSupported( irr::video::EDT_DIRECT3D8 ) );
				
				irr::core::stringw burningsTextString = L"Software 1";
				auto burningsTextDimensions = font->getDimension( burningsTextString.c_str() );
				auto burningsTextRectangle = Rectangle( d3d8TextRectangle.LowerRightCorner.X, d3d8TextRectangle.UpperLeftCorner.Y, d3d8TextRectangle.LowerRightCorner.X + burningsTextDimensions.Width + 30, d3d8TextRectangle.UpperLeftCorner.Y + burningsTextDimensions.Height );
				burningsSoftwareCheckBox = environment->addCheckBox( ( settingsManager->driverType == irr::video::EDT_BURNINGSVIDEO ), burningsTextRectangle, graphicsTab, BURNINGSSOFTWARE_CHECKBOX_ID, burningsTextString.c_str() );
				burningsSoftwareCheckBox->setEnabled( device->isDriverSupported( irr::video::EDT_BURNINGSVIDEO ) );
				
				irr::core::stringw irrlichtTextString = L"Software 2";
				auto irrlichtTextDimensions = font->getDimension( irrlichtTextString.c_str() );
				auto irrlichtTextRectangle = Rectangle( burningsTextRectangle.LowerRightCorner.X, burningsTextRectangle.UpperLeftCorner.Y, burningsTextRectangle.LowerRightCorner.X + irrlichtTextDimensions.Width + 30, burningsTextRectangle.UpperLeftCorner.Y + irrlichtTextDimensions.Height );
				irrlichtSoftwareCheckBox = environment->addCheckBox( ( settingsManager->driverType == irr::video::EDT_SOFTWARE ), irrlichtTextRectangle, graphicsTab, IRRLICHTSOFTWARE_CHECKBOX_ID, irrlichtTextString.c_str() );
				irrlichtSoftwareCheckBox->setEnabled( device->isDriverSupported( irr::video::EDT_SOFTWARE ) );
				
				itemY = irrlichtTextRectangle.LowerRightCorner.Y + 1;
				
				irr::core::stringw showBackgroundsText = L"Show backgrounds";
				auto showBackgroundsTextDimensions = font->getDimension( showBackgroundsText.c_str() );
				auto showBackgroundsRectangle = Rectangle( 0, itemY, 0 + showBackgroundsTextDimensions.Width + 30, itemY + showBackgroundsTextDimensions.Height );
				showBackgroundsCheckBox = environment->addCheckBox( settingsManager->showBackgrounds, showBackgroundsRectangle, graphicsTab, SHOW_BACKGROUNDS_CHECKBOX_ID, showBackgroundsText.c_str() );
				
				irr::core::stringw backgroundAnimationsText = L"Background animations";
				auto backgroundAnimationsTextDimensions = font->getDimension( backgroundAnimationsText.c_str() );
				auto backgroundAnimationsRectangle = Rectangle( showBackgroundsRectangle.LowerRightCorner.X, showBackgroundsRectangle.UpperLeftCorner.Y, showBackgroundsRectangle.LowerRightCorner.X + backgroundAnimationsTextDimensions.Width + 30, showBackgroundsRectangle.UpperLeftCorner.Y + backgroundAnimationsTextDimensions.Height );
				backgroundAnimationsCheckBox = environment->addCheckBox( settingsManager->backgroundAnimations, backgroundAnimationsRectangle, graphicsTab, BACKGROUND_ANIMATIONS_CHECKBOX_ID, backgroundAnimationsText.c_str() );
				backgroundAnimationsCheckBox->setEnabled( showBackgroundsCheckBox->isChecked() );
				
				itemY = backgroundAnimationsRectangle.LowerRightCorner.Y + 1;
				
				irr::core::stringw markTrailsText = L"Mark player trails";
				auto markTrailsTextDimensions = font->getDimension( markTrailsText.c_str() );
				auto markTrailsRectangle = Rectangle( 0, itemY, 0 + markTrailsTextDimensions.Width + 30, itemY + markTrailsTextDimensions.Height );
				markTrailsCheckBox = environment->addCheckBox( settingsManager->markTrails, markTrailsRectangle, graphicsTab, MARK_TRAILS_CHECKBOX_ID, markTrailsText.c_str() );
				
				itemY = markTrailsRectangle.LowerRightCorner.Y + 1;
			}
			
			{ //Multiplayer tab
				decltype( mainGame->getScreenSize().Height ) itemY = 0;
				
				irr::core::stringw numPlayersTextString = L"Number of players";
				auto numPlayersTextDimensions = font->getDimension( numPlayersTextString.c_str() );
				auto numPlayersTextRectangle = Rectangle( 0, itemY, 0 + numPlayersTextDimensions.Width, itemY + numPlayersTextDimensions.Height );
				numPlayersText = environment->addStaticText( numPlayersTextString.c_str(), numPlayersTextRectangle, false, true, multiplayerTab );
				
				irr::core::stringw numPlayersSpinboxText = L"255";
				auto numPlayersSpinboxDimensions = font->getDimension( numPlayersSpinboxText.c_str() );
				numPlayersSpinboxDimensions.Width *= 2; //No idea why, but Irrlicht 1.8 seems to ignore the width if we don't do this
				auto numPlayersSpinboxRectangle = Rectangle( mainGame->getScreenSize().Width - numPlayersSpinboxDimensions.Width, itemY, mainGame->getScreenSize().Width, itemY + numPlayersSpinboxDimensions.Height );
				{
					StringConverter sc;
					numPlayersSpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->numPlayers ).c_str(), numPlayersSpinboxRectangle, true, multiplayerTab, NUM_PLAYERS_SPINBOX_ID );
				}
				numPlayersSpinBox->setRange( 1, 255 );
				numPlayersSpinBox->setValue( settingsManager->numPlayers );
				numPlayersSpinBox->setDecimalPlaces( 0 );
				
				auto numPlayersBarRectangle = Rectangle( numPlayersTextRectangle.LowerRightCorner.X + 1, itemY, numPlayersSpinboxRectangle.UpperLeftCorner.X, numPlayersTextRectangle.LowerRightCorner.Y );
				numPlayersBar = environment->addScrollBar( true, numPlayersBarRectangle, multiplayerTab, NUM_PLAYERS_BAR_ID );
				numPlayersBar->setMax( numPlayersSpinBox->getMax() );
				numPlayersBar->setMin( numPlayersSpinBox->getMin() );
				numPlayersBar->setPos( numPlayersSpinBox->getValue() );
				numPlayersBar->setSmallStep( 1 );
				numPlayersBar->setLargeStep( 10 );
				
				itemY = 1 + std::max( std::max( numPlayersTextRectangle.LowerRightCorner.Y, numPlayersSpinboxRectangle.LowerRightCorner.Y ), numPlayersBarRectangle.LowerRightCorner.Y );
				
				irr::core::stringw numBotsTextString = L"Number of bots";
				auto numBotsTextDimensions = font->getDimension( numBotsTextString.c_str() );
				auto numBotsTextRectangle = Rectangle( 0, itemY, 0 + numBotsTextDimensions.Width, itemY +numBotsTextDimensions.Height );
				numBotsText = environment->addStaticText( numBotsTextString.c_str(), numBotsTextRectangle, false, true, multiplayerTab );
				
				irr::core::stringw numBotsSpinboxText = L"255";
				auto numBotsSpinboxDimensions = font->getDimension( numBotsSpinboxText.c_str() );
				numBotsSpinboxDimensions.Width *= 2;
				auto numBotsSpinboxRectangle = Rectangle( mainGame->getScreenSize().Width - numBotsSpinboxDimensions.Width, itemY, mainGame->getScreenSize().Width, itemY + numBotsSpinboxDimensions.Height );
				{
					StringConverter sc;
					numBotsSpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->numBots ).c_str(), numBotsSpinboxRectangle, true, multiplayerTab, NUM_BOTS_SPINBOX_ID );
				}
				numBotsSpinBox->setRange( 0, numPlayersSpinBox->getValue() );
				numBotsSpinBox->setValue( settingsManager->numBots );
				numBotsSpinBox->setDecimalPlaces( 0 );
				
				auto numBotsBarRectangle = Rectangle( numBotsTextRectangle.LowerRightCorner.X + 1, itemY, numBotsSpinboxRectangle.UpperLeftCorner.X, numBotsTextRectangle.LowerRightCorner.Y );
				numBotsBar = environment->addScrollBar( true, numBotsBarRectangle, multiplayerTab, NUM_BOTS_BAR_ID );
				numBotsBar->setMax( numBotsSpinBox->getMax() );
				numBotsBar->setMin( numBotsSpinBox->getMin() );
				numBotsBar->setPos( numBotsSpinBox->getValue() );
				numBotsBar->setSmallStep( 1 );
				numBotsBar->setLargeStep( 10 );
				
				itemY = 1 + std::max( std::max( numBotsTextRectangle.LowerRightCorner.Y, numBotsSpinboxRectangle.LowerRightCorner.Y ), numBotsBarRectangle.LowerRightCorner.Y );
				
				irr::core::stringw networkPortTextString = L"Network port";
				auto networkPortTextDimensions = font->getDimension( networkPortTextString.c_str() );
				auto networkPortTextRectangle = Rectangle( 0, itemY, 0 + networkPortTextDimensions.Width, itemY + networkPortTextDimensions.Height );
				networkPortText = environment->addStaticText( networkPortTextString.c_str(), networkPortTextRectangle, false, true, multiplayerTab );
				
				irr::core::stringw networkPortSpinboxText = L"65535";
				auto networkPortSpinboxDimensions = font->getDimension( networkPortSpinboxText.c_str() );
				networkPortSpinboxDimensions.Width *= 2;
				auto networkPortSpinboxRectangle = Rectangle( networkPortTextRectangle.LowerRightCorner.X, networkPortTextRectangle.UpperLeftCorner.Y, networkPortTextRectangle.LowerRightCorner.X + networkPortSpinboxDimensions.Width, networkPortTextRectangle.UpperLeftCorner.Y + networkPortSpinboxDimensions.Height );
				{
					StringConverter sc;
					networkPortSpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->networkPort ).c_str(), networkPortSpinboxRectangle, true, multiplayerTab, NETWORK_PORT_SPINBOX_ID );
				}
				networkPortSpinBox->setRange( 1, 65535 ); //We don't allow 0 because we may or may not use TCP, and TCP disallows the use of port 0.
				networkPortSpinBox->setValue( settingsManager->networkPort );
				networkPortSpinBox->setDecimalPlaces( 0 );
				
				itemY = 1 + std::max( networkPortTextRectangle.LowerRightCorner.Y, networkPortSpinboxRectangle.LowerRightCorner.Y );
				
				irr::core::stringw alwaysServerText = L"Always server";
				auto alwaysServerTextDimensions = font->getDimension( alwaysServerText.c_str() );
				auto alwaysServerRectangle = Rectangle( 0, itemY, 0 + 30 + alwaysServerTextDimensions.Width, itemY + alwaysServerTextDimensions.Height );
				alwaysServerCheckBox = environment->addCheckBox( settingsManager->alwaysServer, alwaysServerRectangle, multiplayerTab, ALWAYS_SERVER_CHECKBOX_ID, alwaysServerText.c_str() );
				
				itemY = alwaysServerRectangle.LowerRightCorner.Y + 1;
				
				irr::core::stringw botMovementDelayTextString = L"Bot movement delay";
				auto botMovementDelayTextDimensions = font->getDimension( botMovementDelayTextString.c_str() );
				auto botMovementDelayTextRectangle = Rectangle( 0, itemY, 0 + botMovementDelayTextDimensions.Width, itemY + botMovementDelayTextDimensions.Height );
				botMovementDelayText = environment->addStaticText( botMovementDelayTextString.c_str(), botMovementDelayTextRectangle, false, true, multiplayerTab );
				
				irr::core::stringw botMovementDelaySpinBoxText = L"999";
				auto botMovementDelaySpinBoxDimensions = font->getDimension( botMovementDelaySpinBoxText.c_str() );
				botMovementDelaySpinBoxDimensions.Width *= 2;
				auto botMovementDelaySpinBoxRectangle = Rectangle( botMovementDelayTextRectangle.LowerRightCorner.X, botMovementDelayTextRectangle.UpperLeftCorner.Y, botMovementDelayTextRectangle.LowerRightCorner.X + botMovementDelayTextDimensions.Width, botMovementDelayTextRectangle.UpperLeftCorner.Y + botMovementDelayTextDimensions.Height );
				{
					StringConverter sc;
					botMovementDelaySpinBox = environment->addSpinBox( sc.toStdWString( settingsManager->botMovementDelay ).c_str(), botMovementDelaySpinBoxRectangle, true, multiplayerTab, BOT_MOVEMENT_DELAY_SPINBOX_ID );
				}
				botMovementDelaySpinBox->setRange( 0, 65535 );
				botMovementDelaySpinBox->setValue( settingsManager->botMovementDelay );
				botMovementDelaySpinBox->setDecimalPlaces( 0 );
				
				itemY = 1 + std::max( botMovementDelayTextRectangle.LowerRightCorner.Y, botMovementDelaySpinBoxRectangle.LowerRightCorner.Y );
			}
			
			{ //Miscellaneous tab
				
			}
		}
	} catch( std::exception e ) {
		std::wcerr << L"Error in SettingsScreen::changeToSettingsScreen(): " << e.what() << std::endl;
	}
}

void SettingsScreen::changeFromSettingsScreen() {
	environment->clear();
}

void SettingsScreen::draw( irr::IrrlichtDevice* device ) {
	/*irr::core::dimension2d< irr::u32 > tempDimensions = textFont->getDimension( restartNotice.c_str() );
	Rectangle tempRectangle( 0, 0, tempDimensions.Width + 0, tempDimensions.Height + 0 );
	textFont->draw( restartNotice, tempRectangle, WHITE, true, true );
	cancel.draw( device );
	ok.draw( device );
	undoChanges.draw( device );
	resetToDefaults.draw( device );*/
	environment->drawAll();
}

bool SettingsScreen::OnEvent( const irr::SEvent& event ) {
	try {
		switch( event.EventType ) {
			case irr::EET_GUI_EVENT: {
				auto id = event.GUIEvent.Caller->getID();
				
				switch( event.GUIEvent.EventType ) {
					case irr::gui::EGET_SPINBOX_CHANGED: {
						switch( id ) {
							case BOT_MOVEMENT_DELAY_SPINBOX_ID: {
								settingsChanged = true;
								settingsManager->botMovementDelay = botMovementDelaySpinBox->getValue();
								break;
							}
							case NETWORK_PORT_SPINBOX_ID: {
								settingsManager->networkPort = networkPortSpinBox->getValue();
								settingsChanged = true;
								break;
							}
							case NUM_BOTS_SPINBOX_ID: {
								numBotsBar->setPos( numBotsSpinBox->getValue() );
								settingsManager->numBots = numBotsSpinBox->getValue();
								settingsChanged = true;
								break;
							}
							case NUM_PLAYERS_SPINBOX_ID: {
								numPlayersBar->setPos( numPlayersSpinBox->getValue() );
								numBotsBar->setMax( numPlayersSpinBox->getValue() );
								numBotsSpinBox->setRange( numBotsSpinBox->getMin(), numPlayersSpinBox->getValue() );
								settingsManager->numPlayers = numPlayersSpinBox->getValue();
								settingsChanged = true;
								break;
							}
							case FULLSCREEN_WIDTH_SPINBOX_ID: {
								auto temp = settingsManager->getFullscreenResolution();
								temp.Width = fullscreenWidthSpinBox->getValue();
								settingsManager->setFullscreenResolution( temp );
								settingsChanged = true;
								break;
							}
							case FULLSCREEN_HEIGHT_SPINBOX_ID: {
								auto temp = settingsManager->getFullscreenResolution();
								temp.Height = fullscreenWidthSpinBox->getValue();
								settingsManager->setFullscreenResolution( temp );
								settingsChanged = true;
								break;
							}
							case WINDOW_WIDTH_SPINBOX_ID: {
								auto temp = settingsManager->getWindowSize();
								temp.Width = windowWidthSpinBox->getValue();
								settingsManager->setWindowSize( temp );
								settingsChanged = true;
								break;
							}
							case WINDOW_HEIGHT_SPINBOX_ID: {
								auto temp = settingsManager->getWindowSize();
								temp.Height = windowWidthSpinBox->getValue();
								settingsManager->setWindowSize( temp );
								settingsChanged = true;
								break;
							}
							default: {
								CustomException e( L"Unhandled spinbox ID" );
								throw( e );
							}
						}
						break;
					}
					case irr::gui::EGET_BUTTON_CLICKED: {
						switch( id ) {
							case CANCEL_ID: {
								if( settingsChanged ) {
									resetChangedSettings();
								}
								backToMenu();
								return true;
							}
							case OK_ID: {
								if( settingsChanged ) {
									saveSettings();
								}
								backToMenu();
								return true;
							}
							case RESET_TO_DEFAULTS_ID: {
								resetToDefaultSettings();
								return true;
							}
							case UNDO_CHANGES_ID: {
								resetChangedSettings();
								return true;
							}
							default: {
								CustomException e( L"Unhandled button ID" );
								throw( e );
							}
						}
						
						break;
					}
					case irr::gui::EGET_CHECKBOX_CHANGED: {
						switch( id ) {
							case ALWAYS_SERVER_CHECKBOX_ID: {
								settingsChanged = true;
								settingsManager->alwaysServer = alwaysServerCheckBox->isChecked();
								break;
							}
							case PLAY_MUSIC_CHECKBOX_ID: {
								settingsChanged = true;
								settingsManager->setPlayMusic( playMusicCheckBox->isChecked() );
								volumeText->setEnabled( playMusicCheckBox->isChecked() );
								volumeBar->setEnabled( playMusicCheckBox->isChecked() );
								break;
							}
							case AUTODETECT_RESOLUTION_CHECKBOX_ID: {
								settingsChanged = true;
								fullscreenHeightSpinBox->setEnabled( autoDetectResolutionCheckBox->isEnabled() && !autoDetectResolutionCheckBox->isChecked() );
								fullscreenWidthSpinBox->setEnabled( autoDetectResolutionCheckBox->isEnabled() && !autoDetectResolutionCheckBox->isChecked() );
								break;
							}
							case FULLSCREEN_CHECKBOX_ID: {
								settingsChanged = true;
								settingsManager->fullscreen = fullscreenCheckBox->isChecked();
								bppText->setEnabled( fullscreenCheckBox->isChecked() );
								bpp16CheckBox->setEnabled( fullscreenCheckBox->isChecked() );
								bpp32CheckBox->setEnabled( fullscreenCheckBox->isChecked() );
								autoDetectResolutionCheckBox->setEnabled( fullscreenCheckBox->isChecked() );
								fullscreenResolutionText->setEnabled( fullscreenCheckBox->isChecked() );
								
								fullscreenHeightSpinBox->setEnabled( autoDetectResolutionCheckBox->isEnabled() && !autoDetectResolutionCheckBox->isChecked() );
								fullscreenWidthSpinBox->setEnabled( autoDetectResolutionCheckBox->isEnabled() && !autoDetectResolutionCheckBox->isChecked() );
								
								windowSizeText->setEnabled( !fullscreenCheckBox->isChecked() );
								windowWidthSpinBox->setEnabled( !fullscreenCheckBox->isChecked() );
								windowHeightSpinBox->setEnabled( !fullscreenCheckBox->isChecked() );
								break;
							}
							case BPP16_CHECKBOX_ID: {
								bpp32CheckBox->setChecked( !bpp16CheckBox->isChecked() );
								//break; Deliberate fall-through here
							}
							case BPP32_CHECKBOX_ID: {
								settingsChanged = true;
								if( bpp32CheckBox->isChecked() ) {
									settingsManager->setBitsPerPixel( 32 );
								} else {
									settingsManager->setBitsPerPixel( 16 );
								}
								
								bpp16CheckBox->setChecked( !bpp32CheckBox->isChecked() );
								break;
							}
							case VSYNC_CHECKBOX_ID: {
								settingsChanged = true;
								settingsManager->vsync = vsyncCheckBox->isChecked();
								break;
							}
							case OPENGL_CHECKBOX_ID: {
								settingsChanged = true;
								if( openGLCheckBox->isChecked() ) {
									settingsManager->driverType = irr::video::EDT_OPENGL;
									direct3D8CheckBox->setChecked( false );
									direct3D9CheckBox->setChecked( false );
									burningsSoftwareCheckBox->setChecked( false );
									irrlichtSoftwareCheckBox->setChecked( false );
								}
								break;
							}
							case DIRECT3D9_CHECKBOX_ID: {
								settingsChanged = true;
								if( direct3D9CheckBox->isChecked() ) {
									settingsManager->driverType = irr::video::EDT_DIRECT3D9;
									direct3D8CheckBox->setChecked( false );
									openGLCheckBox->setChecked( false );
									burningsSoftwareCheckBox->setChecked( false );
									irrlichtSoftwareCheckBox->setChecked( false );
								}
								break;
							}
							case DIRECT3D8_CHECKBOX_ID: {
								settingsChanged = true;
								if( direct3D8CheckBox->isChecked() ) {
									settingsManager->driverType = irr::video::EDT_DIRECT3D8;
									openGLCheckBox->setChecked( false );
									direct3D9CheckBox->setChecked( false );
									burningsSoftwareCheckBox->setChecked( false );
									irrlichtSoftwareCheckBox->setChecked( false );
								}
								break;
							}
							case BURNINGSSOFTWARE_CHECKBOX_ID: {
								settingsChanged = true;
								if( burningsSoftwareCheckBox->isChecked() ) {
									settingsManager->driverType = irr::video::EDT_BURNINGSVIDEO;
									direct3D8CheckBox->setChecked( false );
									direct3D9CheckBox->setChecked( false );
									openGLCheckBox->setChecked( false );
									irrlichtSoftwareCheckBox->setChecked( false );
								}
								break;
							}
							case IRRLICHTSOFTWARE_CHECKBOX_ID: {
								settingsChanged = true;
								if( irrlichtSoftwareCheckBox->isChecked() ) {
									settingsManager->driverType = irr::video::EDT_SOFTWARE;
									direct3D8CheckBox->setChecked( false );
									direct3D9CheckBox->setChecked( false );
									burningsSoftwareCheckBox->setChecked( false );
									openGLCheckBox->setChecked( false );
								}
								break;
							}
							case SHOW_BACKGROUNDS_CHECKBOX_ID: {
								settingsChanged = true;
								settingsManager->showBackgrounds = showBackgroundsCheckBox->isChecked();
								backgroundAnimationsCheckBox->setEnabled( showBackgroundsCheckBox->isChecked() );
								break;
							}
							case BACKGROUND_ANIMATIONS_CHECKBOX_ID: {
								settingsChanged = true;
								settingsManager->backgroundAnimations = backgroundAnimationsCheckBox->isChecked();
								break;
							}
							case MARK_TRAILS_CHECKBOX_ID: {
								settingsChanged = true;
								settingsManager->markTrails = markTrailsCheckBox->isChecked();
								break;
							}
							default: {
								CustomException e( L"Unhandled checkbox ID" );
								throw( e );
							}
						}
						
						break;
					}
					case irr::gui::EGET_SCROLL_BAR_CHANGED: {
						switch( id ) {
							case NUM_BOTS_BAR_ID: {
								numBotsSpinBox->setValue( numBotsBar->getPos() );
								settingsManager->numBots = numBotsBar->getPos();
								settingsChanged = true;
								break;
							}
							case NUM_PLAYERS_BAR_ID: {
								numPlayersSpinBox->setValue( numPlayersBar->getPos() );
								numBotsBar->setMax( numPlayersSpinBox->getValue() );
								numBotsSpinBox->setRange( numBotsSpinBox->getMin(), numPlayersSpinBox->getValue() );
								settingsManager->numPlayers = numPlayersBar->getPos();
								settingsChanged = true;
								break;
							}
							case VOLUME_BAR_ID: {
								settingsChanged = true;
								settingsManager->setMusicVolume( volumeBar->getPos() );
								break;
							}
							default: {
								CustomException e( L"Unhandled scroll bar ID" );
								throw( e );
							}
						}
						break;
					}
					default: {
						CustomException e( L"Unhandled event.GUIEvent.EventType" );
						throw( e );
					}
				}
				
				break;
			}
			default: {
				/*CustomException e( L"Unhandled event.EventType" );
				throw( e );*/
				return false;
			}
		}
	} catch( CustomException e ) {
		std::wcerr << L"Error in SettingsScreen::OnEvent(): " << e.what() << std::endl;
		return false;
	}
	return false; //Control should never actually get this far, but gcc thinks it will so I'm putting this return statement here.
}

/*void SettingsScreen::processSelection() {
	if( cancel.highlighted ) {
		std::wcout << L"Cancel button pressed" << std::endl;
		resetChangedSettings();
		backToMenu();
	} else if( ok.highlighted ) {
		std::wcout << L"OK button pressed" << std::endl;
		if( settingsChanged ) {
			saveSettings();
		}
		backToMenu();
	} else if( undoChanges.highlighted ) {
		std::wcout << L"Undo changes button pressed" << std::endl;
		resetChangedSettings();
	} else if( resetToDefaults.highlighted ) {
		std::wcout << L"Reset to defaults button pressed" << std::endl;
		resetToDefaultSettings();
	}
}*/

void SettingsScreen::resetChangedSettings() {
	settingsChanged = false;
	settingsManager->readPrefs();
}

void SettingsScreen::resetToDefaultSettings() {
	std::wcerr << L"SettingsScreen::resetToDefaultSettings() not fully implemented yet." << std::endl;
	settingsChanged = true;
	settingsManager->resetToDefaults();
}

void SettingsScreen::saveSettings() {
	settingsManager->savePrefs();
}

void SettingsScreen::setButtonFont( irr::gui::IGUIFont* newButtonFont ) {
	buttonFont = newButtonFont;
}

void SettingsScreen::setPointers( MainGame* newMainGame, irr::IrrlichtDevice* newDevice, irr::gui::IGUIFont* newButtonFont, irr::gui::IGUIFont* newTextFont, SettingsManager* newSettingsManager ) {
	mainGame = newMainGame;
	device = newDevice;
	setButtonFont( newButtonFont );
	setTextFont( newTextFont );
	settingsManager = newSettingsManager;
}

void SettingsScreen::setTextFont( irr::gui::IGUIFont* newTextFont ) {
	textFont = newTextFont;
}

void SettingsScreen::setupIconsAndStuff() {
	/*cancel.setX( 0 );
	
	if( buttonFont != NULL && buttonFont != nullptr ) {
		cancel.setY( textFont->getDimension( restartNotice.c_str() ).Height + 5 );
	} else {
		cancel.setY( 5 );
	}
	cancel.setType( device, MenuOption::CANCEL );
	cancel.setFontAndResizeIcon( device, buttonFont );
	cancel.loadTexture( device );
	
	ok.setY( cancel.getY() );
	ok.setX( cancel.getX() + cancel.getWidth() + 1 );
	ok.setType( device, MenuOption::OK );
	ok.setFontAndResizeIcon( device, buttonFont );
	ok.loadTexture( device );
	
	undoChanges.setY( cancel.getY() );
	undoChanges.setX( ok.getX() + ok.getWidth() + 1 );
	undoChanges.setType( device, MenuOption::UNDO_CHANGES );
	undoChanges.setFontAndResizeIcon( device, buttonFont );
	undoChanges.loadTexture( device );
	
	resetToDefaults.setY( cancel.getY() );
	resetToDefaults.setX( undoChanges.getX() + undoChanges.getWidth() + 1 );
	resetToDefaults.setType( device, MenuOption::RESET_TO_DEFAULTS );
	resetToDefaults.setFontAndResizeIcon( device, buttonFont );
	resetToDefaults.loadTexture( device );*/
}