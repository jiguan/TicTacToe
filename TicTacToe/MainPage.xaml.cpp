//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace TicTacToe;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
    InitializeComponent();
}

void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
    (void)e;
}

void TicTacToe::MainPage::displayResult(Player const player)
{
    Platform::String^ text = nullptr;
    switch (player)
    {
    case Player::None:
        text = "It's a draw!";
        break;
    case Player::Computer:
        text = "Computer wins!";
        break;
    case Player::User:
        text = "User wins!";
        break;
    }

    txtStatus->Text = text;

    // If it is not a draw, then a line covering 3 marks should get displayed
    if (player != Player::None)
    {
        auto coordinates = game.getWinningLine();
        if (coordinates.first.isValid() && coordinates.second.isValid())
        {
            placeCut(coordinates.first, coordinates.second);
        }
    }
}

// Draw the mark at the cell for the player or computer
void TicTacToe::MainPage::placeMark(Cell const cell, Player const player)
{
    auto image = ref new Image();
    auto bitmap = ref new BitmapImage(
        ref new Uri(player == Player::Computer ? "ms-appx:///Assets/tttx.bmp" : "ms-appx:///Assets/ttt0.bmp"));
    bitmap->ImageOpened += ref new RoutedEventHandler(
        [this, image, bitmap, cell](Object^ sender, RoutedEventArgs^ e) {
        image->Width = bitmap->PixelWidth;
        image->Height = bitmap->PixelHeight;
        image->Visibility = Windows::UI::Xaml::Visibility::Visible;
    });

    image->Source = bitmap;

    image->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
    image->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Center;
    image->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;

    Grid::SetRow(image, cell.row);
    Grid::SetColumn(image, cell.col);

    board->Children->Append(image);
}

// Draw the line covering victory marks.
void TicTacToe::MainPage::placeCut(Cell const start, Cell const end)
{
    auto cellWidth = board->ActualWidth / 3;
    auto cellHeight = board->ActualHeight / 3;

    auto line = ref new Line();
    line->X1 = start.col * cellWidth + cellWidth / 2;
    line->Y1 = start.row * cellHeight + cellHeight / 2;

    line->X2 = end.col * cellWidth + cellWidth / 2;
    line->Y2 = end.row * cellHeight + cellHeight / 2;

    line->StrokeStartLineCap = Windows::UI::Xaml::Media::PenLineCap::Round;
    line->StrokeEndLineCap = Windows::UI::Xaml::Media::PenLineCap::Round;
    line->StrokeThickness = 15;
    line->Stroke = ref new SolidColorBrush(Windows::UI::Colors::Red);

    line->Visibility = Windows::UI::Xaml::Visibility::Visible;

    Grid::SetRow(line, 0);
    Grid::SetColumn(line, 0);
    Grid::SetRowSpan(line, 3);
    Grid::SetColumnSpan(line, 3);

    board->Children->Append(line);
}

void TicTacToe::MainPage::resetGame() {
    std::vector<Windows::UI::Xaml::UIElement^> children;

    for (auto const & child : board->Children)
    {
        auto typeName = child->GetType()->FullName;
        // Only remove marks and cuts.
        if (typeName == "Windows.UI.Xaml.Controls.Image" || typeName == "Windows.UI.Xaml.Shapes.Line")
        {
            // Record their indexes. 
            children.push_back(child);
        }
    }

    for (auto const & child : children)
    {
        unsigned int index;
        if (board->Children->IndexOf(child, &index))
        {
            board->Children->RemoveAt(index);
        }
    }

    txtStatus->Text = nullptr;
}

// Either 'Start user' or 'Star computer' button clicks, a new game starts. 
void TicTacToe::MainPage::btnStartUser_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
    resetGame();

    game.start(Player::User);
}

void TicTacToe::MainPage::btnStartComputer_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
    resetGame();

    game.start(Player::Computer);
    auto cell = game.move(Player::Computer);
    placeMark(cell, Player::Computer);
}

// Occurs when a pressed mouse button is released, or a touch or pen contact is lifted from the digitizer surface, within the bounding rectangle of the app.
void TicTacToe::MainPage::board_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
    if (game.isStarted() && !game.isFinished())
    {
        auto cell_w = board->ActualWidth / 3;
        auto cell_h = board->ActualHeight / 3;

        auto point = e->GetCurrentPoint(board);
        auto row = static_cast<int>(point->Position.Y / cell_h);
        auto col = static_cast<int>(point->Position.X / cell_w);

        game.move(Cell(row, col), Player::User);

        // User is always using mark (O).
        placeMark(Cell(row, col), Player::User);

        if (!game.isFinished())
        {
            auto cell = game.move(Player::Computer);
            placeMark(cell, Player::Computer);
        }
        
        if (game.isFinished())
        {
            displayResult(
                game.isVictory(Player::Computer) ? Player::Computer : Player::None
            );
        }
    }
}