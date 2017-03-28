//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once
#include "game.h"
#include "MainPage.g.h"

namespace TicTacToe
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
    private:
        Game game;
        void placeCut(Cell const start, Cell const end);
        void placeMark(Cell const cell, Player const player);
        void displayResult(Player const player);
        void resetGame();

        void btnStartUser_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void btnStartComputer_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void board_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	};
}
