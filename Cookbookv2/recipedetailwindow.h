#ifndef RECIPEDETAILWINDOW_H
#define RECIPEDETAILWINDOW_H

#include <QWidget>
#include "models/Recipe.h"

class QTextEdit;
class QLabel;

class RecipeDetailWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RecipeDetailWindow(QWidget *parent = nullptr);
    void setRecipeData(const Recipe& recipe, const QStringList& ingredients, const QStringList& steps);

private:
    QLabel *titleLabel;
    QTextEdit *detailsText;
};

#endif // RECIPEDETAILWINDOW_H
