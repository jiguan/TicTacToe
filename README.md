# TicTacToe

This project is to practice Windows development using C++/CX. It is based on [C++ is fun: Writing a Tic Tac Toe Game](https://www.codeproject.com/articles/678078/cplusplus-is-fun-writing-a-tic-tac-toe-game) WinRT part. I renamed some function and variables to have good naming convention. I also added some necessary comments to help understanding. 

### How to run
Build -> Build Solution or press F6 inside Visual Studio, then hit run on local machine.

### Some pitfalls
 - Select the right template. When create a new project, you need to select Templates -> Visual C++ -> Blank App (Universal Windows). An easy way to check you got the right one: go to `MainPage.xaml.cpp`, you should see `MainPage::MainPage() { InitializeComponent(); }`
without warning sign. 
 - Copy resources to the folder. If there are images or sound for the app, copy them to the **folder on the disk**. Don't copy inside the Visual Studio, which only copies reference. Unless this is what you want.


 
