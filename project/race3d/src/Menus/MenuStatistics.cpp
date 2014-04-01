#include "MenuStatistics.h"
#include "MenuManager.h"

#include "../GUI/GUILabel.h"

#include "../Utilities/Config/Config.h"

#include "../Localization/Localization.h"

#include "../GameLogic/GameMode/GameMode.h"
#include "../GameLogic/GameMode/GM_Training.h"
#include "../GameLogic/GameMode/GM_TimeAttack.h"

//---------------------------------------------------------------------------------

MenuStatistics::MenuStatistics()
{
	m_typeId = MENUELEMENTTYPE_STATISTICS;

	GetElementByName<GUIFrame*>( m_frame, "menu_statistics" );
	
	GetElementByName<GUILabel*>( m_labelTrainingTimeSpent, "menu_stats_training_timespent_value_label" );
	GetElementByName<GUILabel*>( m_labelTrainingMaxSpeed, "menu_stats_training_maxspeed_value_label" );

	GetElementByName<GUILabel*>( m_labelTimeAttackBestTime, "menu_stats_time_attack_besttime_value_label" );
	GetElementByName<GUILabel*>( m_labelTimeAttackLaps, "menu_stats_time_attack_laps_value_label" );

	m_config = Config::GetInstance();

	// Register this class with GUI Event manager
	GetGUIEventManager()->RegisterEvent( EVENT_BUTTON, dynamic_cast<GUIButtonEventHandler*>( this ) );
}

//---------------------------------------------------------------------------------

MenuStatistics::~MenuStatistics()
{
	m_config->FreeInstance();
}

//---------------------------------------------------------------------------------

void MenuStatistics::Init()
{
}

//---------------------------------------------------------------------------------

void MenuStatistics::Render()
{
}

//---------------------------------------------------------------------------------

void MenuStatistics::Update( const s3e_uint64_t dt )
{
}

//---------------------------------------------------------------------------------

void MenuStatistics::OnShow()
{
	MenuElement::OnShow();
	ShowTrainingStatistics();

	m_menuManager->Fade();
}

//---------------------------------------------------------------------------------

void MenuStatistics::OnHide()
{
	MenuElement::OnShow();
}

//---------------------------------------------------------------------------------

bool MenuStatistics::OnButtonPressed( const GUIButton* apButton )
{
	return false;
}

//---------------------------------------------------------------------------------

bool MenuStatistics::OnButtonReleased( const GUIButton* apButton )
{
	if( apButton->m_Hash == IwHashString( "mstats_back_button" ) )
	{
		m_soundManager->PlaySFXUiSelect();
		m_menuManager->PushMenu( MENUELEMENTTYPE_MAIN_MENU );
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------------

void MenuStatistics::ShowTrainingStatistics()
{
	const std::string minutesStr = LocaliseFindString( "MSTATS_TRAINING_MINUTES" );
	const std::string kmhStr = LocaliseFindString( "MSTATS_TRAINING_KMH" );

	GM_Training* training = static_cast<GM_Training*>( m_config->GetGameMode( GameMode::GM_ID_TRAINING ) );

	const double timeSpent = training->GetTime();
	const float maxSpeed = training->GetMaxSpeed();

	char min[256];
	sprintf( min, "%.0f %s", timeSpent, minutesStr.c_str() );

	char kmh[256];
	sprintf( kmh, "%.0f %s", maxSpeed, kmhStr.c_str() );

	m_labelTrainingTimeSpent->SetText( min );
	m_labelTrainingMaxSpeed->SetText( kmh );

	GM_TimeAttack* timeAttack = static_cast<GM_TimeAttack*>( m_config->GetGameMode( GameMode::GM_ID_TIME_ATTACK ) );

	const double bestTime = timeAttack->GetBestTime();
	const int laps = timeAttack->GetLapsAmount();

	char out[256];
	sprintf( out, "%d", laps );
	m_labelTimeAttackLaps->SetText( out );

	const time_t t = static_cast<time_t>( bestTime * 0.001 );
	if( t > 0 )
	{
		strftime( out, 6, "%M:%S", localtime( &t ) );
		m_labelTimeAttackBestTime->SetText( out );
	}
	else
	{
		m_labelTimeAttackBestTime->SetText( "--:--" );
	}
}

//---------------------------------------------------------------------------------

void MenuStatistics::ShowTimeAttackStatistics()
{
}