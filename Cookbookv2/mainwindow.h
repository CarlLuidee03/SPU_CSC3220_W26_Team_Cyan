#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database/RecipeRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    RecipeRepository m_repository;

    void loadRecipes();
    void onAddRecipeClicked();
    void showRecipeDetails();
    void onDeleteRecipeClicked();
    void onEditRecipeClicked();
};
#endif // MAINWINDOW_H
